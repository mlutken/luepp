#include <stddef.h>
#include <iterator>
#include <stdexcept>
#include <algorithm>
#include <vector>
#include <functional>
#include <iostream>

// --- Ideas, inputs background ---
// https://github.com/aantron/better-enums

namespace estl {

class state_not_found : public std::exception
{
public:
  state_not_found() = default;
  virtual ~state_not_found() = default;
  virtual const char* what() const noexcept { return "State machine state not found"; }
};

template <class SMDEF>
class state_machine
{
public:
    using state_machine_type    = SMDEF;
    using inputs_type           = typename state_machine_type::inputs_type;
    using outputs_type          = typename state_machine_type::outputs_type;
    using state_type            = typename state_machine_type::state_type;
    using switch_state_fun      = std::function<state_type (const inputs_type&) >;
    using actions_fun           = std::function<void (const inputs_type&, outputs_type&) >;
    using sm_actions_memfun     = void (state_machine_type::*)(const inputs_type&, outputs_type&);

    using sm_condition_fun     = std::function<bool (const inputs_type&) >;
    using sm_condition_memfun  = bool (state_machine_type::*)(const inputs_type&);

    class condition {
    public:
        explicit condition(const sm_condition_fun& condition_fun, const state_type& switch_to_state)
            : condition_fun_m(condition_fun), switch_to_state_m(switch_to_state)
        {
        }

        bool operator()(const inputs_type& inputs)
        {
            if (condition_fun_m) {
                return condition_fun_m(inputs);
            }

            return false;
        }
        state_type switch_to_state() const { return switch_to_state_m; }

    private:
        sm_condition_fun    condition_fun_m;
        state_type          switch_to_state_m = state_type::_NOT_HANDLED_;
    };
    
    class state_conditions {
    public:
        state_conditions() = default;
        state_type operator()(const inputs_type& inputs)
        {
            // --- Logical OR of the conditions ---
            if (!conditions_m.empty()) {
                for (auto condition : conditions_m) {
                    if (condition(inputs)){
                        return condition.switch_to_state();
                    }
                }
            }
            return state_type::_END_;
        }

        bool has_conditions() const     {   return !conditions_m.empty();    }

        state_conditions& switch_to(state_type to)
        {
            current_switch_to_state_m = to;
            return *this;
        }

        state_conditions& when(const sm_condition_memfun& member_fun_condition)
        {
            using namespace std::placeholders;  // for _1, _2, _3...

            auto cond = std::bind(member_fun_condition, state_machine_m, _1);
            conditions_m.emplace_back(cond, current_switch_to_state_m);
            return *this;
        }

        state_conditions& when(const std::string& /*condition_name*/,
                               const sm_condition_fun& cond)
        {
            conditions_m.emplace_back(cond, current_switch_to_state_m);
            return *this;
        }

        state_conditions& otherwise_loop()
        {
            const auto cond = [=](const inputs_type& /*inputs*/) -> bool {
                return true;
            };
            conditions_m.emplace_back(cond, from_state_m);

            return *this;
        }

        void end_transitions() {}

        void fromStateSet(state_type from_state)    { from_state_m = from_state;}
        void stateMachineSet(state_machine_type* sm){ state_machine_m = sm;}
    private:
        std::vector<condition>          conditions_m;
        state_machine_type*             state_machine_m = nullptr;
        state_type                      from_state_m;
        state_type                      current_switch_to_state_m;
    };

    struct test_entry {
        test_entry() = default;
        test_entry(state_type from, state_type to, const inputs_type& i) :
            state_from(from), state_to(to), inputs(i), description("") {}
        test_entry(state_type from, state_type to, const inputs_type& i,
                   const std::string& desc) :
            state_from(from), state_to(to), inputs(i), description(desc){}
        state_type  state_from;
        state_type  state_to;
        inputs_type inputs;
        std::string description;
    };
    using test_vec_type = std::vector<test_entry>;


    // -----------------------------------
    // -- Constructors, initialisation ---
    // -----------------------------------
    state_machine() {
        state_machine_.add_transitions(*this);
        state_machine_.add_actions(*this);
    }

    /** Add state switch/transition function.
        This function is to be called from the state machine definition class's
        add_transitions() function.
        @note It CAN only be called once per state */
    void add_transitions_from(state_type from_state, const switch_state_fun& fn) {
        const auto from = static_cast<size_t>(from_state);
        state_switches_[from] = switch_entry{from_state, fn};
    }

    /** Access conditions for given state.
        This function is to be called from the stame machine definition class's
        add_transitions(). */
    state_conditions& transitions_from(state_type from_state) {
        const auto from = static_cast<size_t>(from_state);
        state_switches_[from].switch_conditions_m.fromStateSet(from_state);
        state_switches_[from].switch_conditions_m.stateMachineSet(&state_machine_);
        return state_switches_[from].switch_conditions_m;
    }

    /** Add state action function.
        This function is called from the stame machine definition class's
        add_transitions() static function. It MUST be called once per
        state*/
    void add_action_function(state_type state, const actions_fun& fn) {
        actions_.emplace_back(state, fn);
    }

    void add_action_function(state_type state, const sm_actions_memfun& member_fun_actions)
    {
        using namespace std::placeholders;  // for _1, _2, _3...

        auto fn = std::bind(member_fun_actions, state_machine_, _1, _2);
        actions_.emplace_back(state, fn);
    }


    // ------------------------
    // -- Runtime functions ---
    // ------------------------

    /** Perform run step of statemachine.
        This function switches state based on inputs and if the
        current state changed, it calls execute_actions.
   */
    void run_step (const inputs_type& inputs) {
        const auto old_state = state_;
        state_ = next_state(state_, inputs);
        if (state_ != old_state) {
            execute_actions(inputs);
        }
    }


    /** Change state based on inputs. */
    void switch_state (const inputs_type& inputs) {
        state_ = next_state(state_, inputs);
    }

    /** Get state from from inputs
        Get a next state based on a (current) state and inputs struct
        instance.
        @param state The current state
        @param inputs Current inputs to state machine
        @return The next state based on the current state and inputs. */
    state_type next_state(state_type state, const inputs_type& inputs) const {
        const auto fn = get_switch_state_function(state);
        if (!fn) {
            throw state_not_found{};
        }
        return fn(inputs);
    }

    /** Fire actions for current state and possibly set outputs.
        @param inputs Current inputs to state machine
        @return The next state based on the current state and inputs. */
    void execute_actions(const inputs_type& inputs) {
        execute_actions(state_, inputs, outputs_);
    }

    /** Fire actions for current state and possibly set outputs.
        @param inputs Current inputs to state machine
        @param outputs Reference to struct with outputs to modify
        @return The next state based on the current state and inputs. */
    void execute_actions(const inputs_type& inputs, outputs_type& outputs) const {
        execute_actions(state_, inputs, outputs);
    }

    /** Fire actions for state and possibly set outputs.
        @param state The current state
        @param inputs Current inputs to state machine
        @param outputs Reference to struct with outputs to modify
        @return The next state based on the current state and inputs. */
    void execute_actions(state_type state, const inputs_type& inputs, outputs_type& outputs) const {
        const auto fn = get_action_function(state);
        if (!fn) {
            throw state_not_found{};
        }
        fn(inputs, outputs);
    }

    // -----------------------------------------------
    // --- Simple setter/getter and info functions ---
    // -----------------------------------------------
    constexpr static size_t states_end  ()     { return static_cast<size_t>(state_type::_END_);}
    constexpr static size_t states_count()     { return states_end();}
    void                    state_set   (state_type state) { state_ = state;                }
    state_type              state       () const    { return state_;                        }
    int32_t                 state_int   () const    { return static_cast<int32_t>(state_);  }
    const outputs_type&     outputs     () const    { return outputs_;  }

    // ---------------------
    // -- Test functions ---
    // ---------------------
    // TODO: Should we move this to a subclass or similar?
    bool test_transitions(const test_vec_type& test_vector, bool print_failing = false) const {
        // Test we have transistions for all states
        if (state_switches_.size() != states_count()) {
            if (print_failing) {
                std::cout   << "ERROR: Wrong number of transition functions:\n"
                            << "Expected <> Found:\n"
                            << states_count()
                            << " != " << state_switches_.size()
                            << std::endl;
            }
            return false;
        }

        // Run test cases
        bool all_ok = true;
        for (const test_entry& tc: test_vector) {
            const state_type state_from = tc.state_from;
            const inputs_type& inputs = tc.inputs;
            const auto out_state = next_state(state_from, inputs);
            if (out_state != tc.state_to) {
                all_ok = false;
                if (print_failing) {
                    std::cout   << "FAILED test: " << tc.description << "\n"
                                << "Expected <> Found: "
                                << static_cast<int32_t>(tc.state_to)
                                << " != " << static_cast<int32_t>(out_state)
                                << std::endl;
                }
            }
        }
        return all_ok;
    }

private:
    // ---------------------------------
    // --- PRIVATE: Helper functions ---
    // ---------------------------------

    /// Lookup state switch function from state.
    /// @see https://en.cppreference.com/w/cpp/algorithm/binary_search
    switch_state_fun get_switch_state_function(state_type from_state) const
    {
        return state_switches_[static_cast<size_t>(from_state)];
    }

    /// Lookup action function from state.
    /// @see https://en.cppreference.com/w/cpp/algorithm/binary_search
    actions_fun get_action_function(state_type from_state) const
    {
        constexpr auto compare = [](const action_entry& lhs, state_type state) -> bool {
            return lhs.state < state;
        };

        const auto it = std::lower_bound(actions_.begin(), actions_.end(), from_state, compare);
        if (it != actions_.end() && it->state == from_state) {
            return it->do_actions;
        }
        return nullptr;
    }

    // -------------------------------------
    // --- PRIVATE: Definitions and data ---
    // -------------------------------------
    struct switch_entry {
        switch_entry () = default;
        switch_entry (state_type st, const switch_state_fun& fn) :
            state_m(st), user_do_switch_function_m(fn), switch_conditions_m() {}

        state_type          state_m;
        switch_state_fun    user_do_switch_function_m;
        state_conditions    switch_conditions_m;

        state_type operator()(const inputs_type& inputs) {
            if (user_do_switch_function_m) {
                return user_do_switch_function_m(inputs);
            }
            else if (switch_conditions_m.has_conditions()) {
                return switch_conditions_m(inputs);
            }
            return state_type::_END_;
        }
    };

    using state_switch_array = std::array<switch_entry, states_count()>;

    struct action_entry {
        action_entry (state_type st, const actions_fun& fn) :
            state(st), do_actions(fn) {}
        state_type    state;
        actions_fun   do_actions;
    };
    using actions_vec = std::vector<action_entry>;

    // --- Member data ---
    state_machine_type  state_machine_;
    state_switch_array  state_switches_;
    actions_vec         actions_;
    state_type          state_ = state_machine_type::initial_state;
    outputs_type        outputs_;
};


} // END namespace cas
