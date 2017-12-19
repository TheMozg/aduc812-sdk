#include "dip.h"
#include "app_echo.h"
#include "app_calc.h"

typedef enum {
    none,
    echo,
    calc
} app_t;

static app_t last_app;

app_t get_driver() {
    if (read_dip() == 0xFF){
        return calc;
    }
    return echo;
}

void main() {
    app_t last_app = none;
    while( 1 ) {
        app_t app = get_driver();

        if (app == echo) {
            if (app != last_app) {
                init_echo_app();
            }
            run_echo_app();
        }

        if (app == calc) {
            if (app != last_app) {
                init_calc_app();
            }
            run_calc_app();
        }

        last_app = app;
    }
}
