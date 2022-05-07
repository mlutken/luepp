#include <iostream>
#include <chrono>
#include <boost/format.hpp>
#include <fsm/state_machine.hpp>

using namespace std;

// ---------------------
// --- Input signals ---
// ---------------------
enum class input_event : unsigned {
    no_btn_presed,
    power_btn_pressed,
    playpause_btn_pressed
};

// ----------------------
// --- Output signals ---
// ----------------------
enum class pwr_light_sig    : unsigned { none, active };

// ----------------------------------------------------------------------
// --- Statemachine Definition: CD player (very oldschool, sorry :) ) ---
// ----------------------------------------------------------------------
class cdplayer {
public:

    // REQUIRED: You must define a state_type enum!
    enum class  state_type : unsigned { off,
                                        on,
                                        playing,
                                        paused,
                                        _END_,
                                        _NOT_HANDLED_
                                      };

    // REQUIRED: You must define an inputs_type struct!
    struct inputs_type {
        using input_event_type = input_event;
        inputs_type() = default;
        inputs_type(input_event act) : event(act) {}
        input_event event = input_event::no_btn_presed;
    };

    // REQUIRED: You must define an outputs_type struct!
    struct outputs_type {
        outputs_type() = default;
        pwr_light_sig   pwr_light           = pwr_light_sig::active;

        std::string debug_str() const {
            const auto f = boost::format("OUTPUTS:\npwr_light: '%1%'\n")
                    % static_cast<int>(pwr_light);
            return f.str();
        }
    };

    // REQUIRED: You must define the initial state!
    static constexpr state_type initial_state = state_type::off;

    cdplayer() = default;

    // REQUIRED: Here you define your transitions!
    void add_transitions(estl::state_machine<cdplayer>& sm)
    {
        // --- Transitions from off ---
        sm.transitions_from(state_type::off)
                .switch_to(state_type::on).when(&cdplayer::power_toggle)
                .otherwise_loop();

        // --- Transitions from on ---
        sm.transitions_from(state_type::on)
                .switch_to(state_type::off).when(&cdplayer::power_toggle)
                .switch_to(state_type::playing).when(&cdplayer::play_pressed)
                .otherwise_loop();

        // --- Transitions from playing ---
        sm.transitions_from(state_type::playing)
                .switch_to(state_type::off).when(&cdplayer::power_toggle)
                .switch_to(state_type::paused).when(&cdplayer::play_pressed)
                .otherwise_loop();

        // --- Transitions from paused ---
        sm.transitions_from(state_type::paused)
                .switch_to(state_type::off).when(&cdplayer::power_toggle)
                .switch_to(state_type::playing).when(&cdplayer::play_pressed)
                .otherwise_loop();
    }

    // ---------------------------
    // --- Condition functions ---
    // ---------------------------
    bool power_toggle(const inputs_type& inputs)
    {
        return inputs.event == input_event::power_btn_pressed;
    }

    bool play_pressed(const inputs_type& inputs)
    {
        return inputs.event == input_event::playpause_btn_pressed;
    }

    // ------------------------------
    // --- State action functions ---
    // ------------------------------
    // REQUIRED: Here you define actions taken in each state!
    void add_actions(estl::state_machine<cdplayer>& sm)
    {
        sm.add_action_function(state_type::off,     &cdplayer::actions_in__off);
        sm.add_action_function(state_type::on,      &cdplayer::actions_in__on);
    }

    void actions_in__off(const inputs_type& /*inputs*/, outputs_type& outputs) {
        outputs.pwr_light           = pwr_light_sig::none;
        cout << "actions_in__off: Nothing to do!" << endl;
    }

    void actions_in__on(const inputs_type& /*inputs*/, outputs_type& outputs) {
        outputs.pwr_light           = pwr_light_sig::active;
        cout << "actions_in__on: Ready to rock!" << endl;
    }
};

// -------------------------
// --- Unit test vectors ---
// -------------------------

using SM = estl::state_machine<cdplayer>;
using state_type = SM::state_type;
using inputs_type = SM::inputs_type;
using outputs_type = SM::outputs_type;
using test_vec_type = SM::test_vec_type;
using time_point = std::chrono::steady_clock::time_point;
using std::chrono::seconds;

static const test_vec_type tv1 = {
    {
        state_type::off, state_type::off,
        {},
        "Test that off - stays off."
    },
    {
        state_type::off, state_type::on,
        input_event::power_btn_pressed,
        "Switching on."
    }
    ,
    {
        state_type::on, state_type::playing,
        input_event::playpause_btn_pressed,
        "Start playing."
    }
    ,
    {
        state_type::playing, state_type::paused,
        input_event::playpause_btn_pressed,
        "Pause in playing."
    }
    ,
    {
        state_type::paused, state_type::playing,
        input_event::playpause_btn_pressed,
        "Resume playing."
    }
};

static const test_vec_type tvec1 = {
    {
        state_type::off, state_type::on,
        input_event::power_btn_pressed,
        "Switching on."
    },
    {
        state_type::on, state_type::playing,
        input_event::playpause_btn_pressed,
        "Start playing."
    }
};

int main()
{
    cout << "--- CD player demo playground ---\n";
    SM sm;
    cout << "states_count() : " << sm.states_count() << "\n";
    cout << "current state  : " << sm.state_int() << "\n";
    cout << "tv1.size()      : " << tv1.size() << "\n";
    const auto all_ok = sm.test_transitions(tv1, true);
    cout << "sm.test_transitions(tv1):" << all_ok << "\n";


    SM::inputs_type inputs;
    sm.state_set(state_type::on);
    sm.execute_actions(inputs);


//    cout << sm.outputs().debug_str() << endl;

    cout << endl;
    return 0;
}

