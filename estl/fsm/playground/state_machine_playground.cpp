#include <iostream>
#include <sstream>
#include <chrono>
#include <fsm/state_machine.hpp>

// CD Player Statemachine example:
// https://lipn.univ-paris13.fr/~benmoussa/Model.html
// clang -I../..  -Xclang -ast-dump -fsyntax-only -fno-color-diagnostics state_machine_playground.cpp > state_machine_playground.cpp.ast
// grep -B 3 add_transition state_machine_playground.cpp.ast
// http://digitocero.com/en/blog/exporting-and-visualizing-gccs-abstract-syntax-tree-ast
//https://github.com/foonathan/cppast


using namespace std;

// --- Input signals ---
enum class power_toggle_sig : unsigned { none, active };
enum class eject_sig        : unsigned { none, active };
enum class play_pause_sig   : unsigned { none, active };
enum class stop_sig         : unsigned { none, active };
enum class has_disc_sig     : unsigned { none, active };
enum class bay_closed_sig   : unsigned { closed, open };

// --- Output signals ---
enum class pwr_light_sig    : unsigned { none, active };
enum class disc_loaded_sig  : unsigned { none, active };
enum class playing_sig      : unsigned { none, active };
enum class open_close_sig   : unsigned { none, opening, closing};

class cd_fsm_def {
public:

    static const int32_t eject_timeout_seconds;


    using time_point = std::chrono::steady_clock::time_point;
    enum class  state_type : unsigned { off,
                                        start_close,
                                        wait_for_closed,
                                        closed,
                                        start_open,
                                        wait_for_open,
                                        open,
                                        loaded,
                                        playing,
                                        paused,
                                        error_closing,
                                        error_opening,
                                        _END_,
                                        _NOT_HANDLED_
                                      };


    struct inputs_type {
        inputs_type() = default;
        inputs_type(power_toggle_sig power_toggle_v)
            : power_toggle(power_toggle_v)
        {}
        inputs_type(time_point t, play_pause_sig play_pause_v)
            : time(t), play_pause(play_pause_v)
        {}
        inputs_type(time_point t, play_pause_sig play_pause_v, has_disc_sig has_disc_v)
            : time(t), play_pause(play_pause_v), has_disc(has_disc_v)
        {}
        inputs_type(time_point t, play_pause_sig play_pause_v, has_disc_sig has_disc_v,
                    bay_closed_sig bay_closed_v)
            : time(t), play_pause(play_pause_v), has_disc(has_disc_v),
              bay_closed(bay_closed_v)
        {}
        inputs_type(time_point t, play_pause_sig play_pause_v, has_disc_sig has_disc_v,
                    bay_closed_sig bay_closed_v, stop_sig stop_v)
            : time(t), play_pause(play_pause_v), has_disc(has_disc_v),
              bay_closed(bay_closed_v), stop(stop_v)
        {}
        inputs_type(time_point t, play_pause_sig play_pause_v, has_disc_sig has_disc_v,
                    bay_closed_sig bay_closed_v, stop_sig stop_v, eject_sig eject_v,
                    power_toggle_sig pwr_v)
            : time(t), play_pause(play_pause_v), has_disc(has_disc_v),
              bay_closed(bay_closed_v), stop(stop_v), eject(eject_v),
              power_toggle(pwr_v)
        {}

        time_point          time            = {};
        play_pause_sig      play_pause      = play_pause_sig::none;
        has_disc_sig        has_disc        = has_disc_sig::none;
        bay_closed_sig      bay_closed      = bay_closed_sig::closed;
        stop_sig            stop            = stop_sig::none;
        eject_sig           eject           = eject_sig::none;
        power_toggle_sig    power_toggle    = power_toggle_sig::none;
    };

    struct outputs_type {
        outputs_type() = default;

        pwr_light_sig   pwr_light           = pwr_light_sig::active;
        disc_loaded_sig disc_loaded_light   = disc_loaded_sig::active;
        playing_sig     playing_light       = playing_sig::none;
        open_close_sig  bay_motor           = open_close_sig::none;

        std::string debug_str() const {
            std::stringstream ss;
            ss << "OUTPUTS:\npwr_light: '" << static_cast<int>(pwr_light)
                    << " disc_loaded_light: '\n"  << static_cast<int>(disc_loaded_light)
                    << " playing_light: '" << static_cast<int>(playing_light)
                    << " bay_motor: '" << static_cast<int>(bay_motor)
                    ;
            return ss.str();
        }
    };

    static constexpr state_type initial_state = state_type::off;

    cd_fsm_def() = default;

    void add_transitions(estl::state_machine<cd_fsm_def>& sm)
    {
        // --- Transitions from off ---
        sm.transitions_from(state_type::off)
                .switch_to(state_type::start_close).when(&cd_fsm_def::power_toggle)
                .otherwise_loop();

        // --- Transitions from start_close ---
        sm.transitions_from(state_type::start_close)
                .switch_to(state_type::wait_for_closed).when("always go to wait_for_closed", [this](const inputs_type& inputs) {
                    time_timeout_ = inputs.time + std::chrono::seconds(eject_timeout_seconds);
                    return true;
                });

        // --- Transitions from wait_for_closed ---
        sm.transitions_from(state_type::wait_for_closed)
                .switch_to(state_type::error_closing).when(&cd_fsm_def::bay_close_timeout)
                .switch_to(state_type::closed).when(&cd_fsm_def::cd_bay_is_closed)
                .otherwise_loop();

        // --- Transitions from closed ---
        sm.transitions_from(state_type::closed)
                .switch_to(state_type::off).when(&cd_fsm_def::power_toggle)
                .switch_to(state_type::start_open).when(&cd_fsm_def::eject_pressed)
                .switch_to(state_type::loaded).when(&cd_fsm_def::disc_detected)
                .otherwise_loop();

        // --- Transitions from start_open ---
        sm.transitions_from(state_type::start_open)
                .switch_to(state_type::wait_for_open).when(&cd_fsm_def::always_goto_wait_for_open)
                ;

        // --- Transitions from wait_for_open ---
        sm.transitions_from(state_type::wait_for_open)
                .switch_to(state_type::error_opening).when(&cd_fsm_def::bay_open_timeout)
                .switch_to(state_type::open).when(&cd_fsm_def::bay_is_open)
                .otherwise_loop();

        // --- Transitions from open ---
        sm.transitions_from(state_type::open)
                .switch_to(state_type::off).when(&cd_fsm_def::power_toggle)
                .switch_to(state_type::start_close).when(&cd_fsm_def::eject_pressed)
                .otherwise_loop();

        // --- Transitions from loaded ---
        sm.transitions_from(state_type::loaded)
                .switch_to(state_type::off).when(&cd_fsm_def::power_toggle)
                .switch_to(state_type::start_open).when(&cd_fsm_def::eject_pressed)
                .switch_to(state_type::playing).when(&cd_fsm_def::play_btn_pressed)
                .otherwise_loop();

        // --- Transitions from playing ---
        sm.transitions_from(state_type::playing)
                .switch_to(state_type::off).when(&cd_fsm_def::power_toggle)
                .switch_to(state_type::start_open).when(&cd_fsm_def::eject_pressed)
                .switch_to(state_type::paused).when(&cd_fsm_def::play_btn_pressed)
                .otherwise_loop();

        // --- Transitions from paused ---
        sm.transitions_from(state_type::paused)
                .switch_to(state_type::off).when(&cd_fsm_def::power_toggle)
                .switch_to(state_type::start_open).when(&cd_fsm_def::eject_pressed)
                .switch_to(state_type::playing).when(&cd_fsm_def::play_btn_pressed)
                .otherwise_loop();

        // --- Transitions from error_closing ---
        sm.transitions_from(state_type::error_closing)
                .switch_to(state_type::start_close).when(&cd_fsm_def::eject_pressed)
                .otherwise_loop();

        // --- Transitions from error_opening ---
        sm.transitions_from(state_type::error_opening)
                .switch_to(state_type::start_open).when(&cd_fsm_def::eject_pressed)
                .otherwise_loop();
    }

    // ---------------------------
    // --- Condition functions ---
    // ---------------------------
    bool power_toggle(const inputs_type& inputs)
    {
        reset_timeout();
        return inputs.power_toggle == power_toggle_sig::active;
    }

    bool bay_close_timeout(const inputs_type& inputs)
    {
        return reached_timeout(inputs.time);
    }

    bool cd_bay_is_closed(const inputs_type& inputs)
    {
        return inputs.bay_closed == bay_closed_sig::closed;
    }

    bool eject_pressed(const inputs_type& inputs)
    {
        reset_timeout();
        return inputs.eject == eject_sig::active;
    }

    bool disc_detected(const inputs_type& inputs)
    {
        return inputs.has_disc == has_disc_sig::active;
    }

    bool always_goto_wait_for_open(const inputs_type& inputs)
    {
        time_timeout_ = inputs.time + std::chrono::seconds(eject_timeout_seconds);
        return true;
    }

    bool bay_open_timeout(const inputs_type& inputs)
    {
        return reached_timeout(inputs.time);
    }

    bool bay_is_open(const inputs_type& inputs)
    {
        return inputs.bay_closed == bay_closed_sig::open;
    }

    bool play_btn_pressed(const inputs_type& inputs)
    {
        return inputs.play_pause == play_pause_sig::active;
    }

    // ------------------------------
    // --- State action functions ---
    // ------------------------------
    void add_actions(estl::state_machine<cd_fsm_def>& sm)
    {
        sm.add_action_function(state_type::off,             &cd_fsm_def::actions_in__off);
        sm.add_action_function(state_type::start_close,     &cd_fsm_def::actions_in__start_close);
        sm.add_action_function(state_type::wait_for_closed, &cd_fsm_def::actions_in__wait_for_closed);
        sm.add_action_function(state_type::closed,          &cd_fsm_def::actions_in__closed);
    }

    void actions_in__off(const inputs_type& /*inputs*/, outputs_type& outputs) {
        outputs.pwr_light           = pwr_light_sig::none;
        outputs.disc_loaded_light   = disc_loaded_sig::none;
        outputs.playing_light       = playing_sig::none;
        outputs.bay_motor           = open_close_sig::none;
        cout << "actions_in__off: Nothing to do!" << endl;
    }

    void actions_in__start_close(const inputs_type& /*inputs*/, outputs_type& outputs) {
        outputs.pwr_light           = pwr_light_sig::active;
        outputs.disc_loaded_light   = disc_loaded_sig::none;
        outputs.playing_light       = playing_sig::none;
        outputs.bay_motor           = open_close_sig::closing;

        cout << "actions_in__start_close"
             << endl;
    }

    void actions_in__wait_for_closed(const inputs_type& /*inputs*/, outputs_type& outputs) {
        outputs.pwr_light           = pwr_light_sig::active;
        outputs.disc_loaded_light   = disc_loaded_sig::none;
        outputs.playing_light       = playing_sig::none;
        outputs.bay_motor           = open_close_sig::closing;

        cout << "actions_in__wait_for_closed"
             << endl;
    }

    void actions_in__closed(const inputs_type& inputs, outputs_type& outputs) {
        if (inputs.has_disc == has_disc_sig::active){
            outputs.disc_loaded_light = disc_loaded_sig::active;
        }
        else {
            outputs.disc_loaded_light = disc_loaded_sig::none;
        }
        cout << "actions_in__closed" << endl;
    }

    // ---------------------------------
    // --- PRIVATE: Helper functions ---
    // ---------------------------------
    bool reached_timeout(const time_point& time) const
    {
        return time > time_timeout_;
    }

    void reset_timeout()
    {
        time_timeout_ = time_point{};
    }

    // ------------------------
    // --- PRIVATE: Members ---
    // ------------------------
    time_point      time_timeout_ = {};
};

const int32_t cd_fsm_def::eject_timeout_seconds = 10;


using SM = estl::state_machine<cd_fsm_def>;
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
        state_type::off, state_type::start_close,
        {power_toggle_sig::active},
        "Switching on test 1"
    },
    {
        state_type::start_close, state_type::wait_for_closed,
        {},
        "Switching on: Go to wait_for_closed always"
    },
    {
        state_type::wait_for_closed, state_type::wait_for_closed,
        {{}, play_pause_sig::none, has_disc_sig::none, bay_closed_sig::open},
        "Closing: Stay in wait_for_closed"
    },
    {
        state_type::wait_for_closed, state_type::error_closing,
        {time_point{} + seconds(11), play_pause_sig::none, has_disc_sig::none, bay_closed_sig::open},
        "Closing timeout error"
    },
    {
        state_type::wait_for_closed, state_type::closed,
        {{}, play_pause_sig::none, has_disc_sig::none, bay_closed_sig::closed},
        "Closing go to closed state"
    },
    {
        state_type::closed, state_type::closed,
        {{}, play_pause_sig::none, has_disc_sig::none, bay_closed_sig::closed},
        "Stay in closed if disc not loaded"
    },
    {
        state_type::closed, state_type::loaded,
        {{}, play_pause_sig::none, has_disc_sig::active, bay_closed_sig::closed},
        "Closed: Go to loaded state if disc is loaded"
    },
    {
        state_type::loaded, state_type::loaded,
        {{}, play_pause_sig::none, has_disc_sig::none, bay_closed_sig::closed},
        "Stay in loaded if not playing or ejecting"
    },
    {
        state_type::loaded, state_type::playing,
        {{}, play_pause_sig::active, has_disc_sig::active, bay_closed_sig::closed},
        "Start playing"
    },
    {
        state_type::playing, state_type::playing,
        {{}, play_pause_sig::none, has_disc_sig::active, bay_closed_sig::closed},
        "Stay in playing if not play_pause pressed"
    },
    {
        state_type::playing, state_type::paused,
        {{}, play_pause_sig::active, has_disc_sig::active, bay_closed_sig::closed},
        "Test playing => paused"
    },
    {
        state_type::paused, state_type::paused,
        {{}, play_pause_sig::none, has_disc_sig::active, bay_closed_sig::closed},
        "Stay in paused if not play_pause pressed"
    },
    {
        state_type::paused, state_type::playing,
        {{}, play_pause_sig::active, has_disc_sig::active, bay_closed_sig::closed},
        "Test paused => playing"
    },
    // ... More tests ...
};

int main()
{
    cout << "--- state_machine playground ---\n";
    SM sm;
    cout << "states_count() : " << sm.states_count() << "\n";
    cout << "current state  : " << sm.state_int() << "\n";
    cout << "tv1.size()      : " << tv1.size() << "\n";
    const auto all_ok = sm.test_transitions(tv1, true);
    cout << "sm.test_transitions(tv1):" << all_ok << "\n";


    SM::inputs_type inputs;
    sm.state_set(state_type::closed);
    inputs.has_disc = has_disc_sig::active;
    sm.execute_actions(inputs);


    cout << sm.outputs().debug_str()
         << endl;

    cout << endl;
    return 0;
}

