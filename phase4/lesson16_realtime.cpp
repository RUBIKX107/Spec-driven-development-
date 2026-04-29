#include <cstdint>
#include <cstdio>
#include <ctime>
#include <chrono>

// ── timing utilities ──────────────────────────────────────
// on real embedded systems this would be a hardware timer
// we simulate with chrono

using Clock = std::chrono::high_resolution_clock;
using Microseconds = std::chrono::microseconds;

// get current time in microseconds
uint64_t get_time_us() {
    auto now = Clock::now();
    return std::chrono::duration_cast<Microseconds>
           (now.time_since_epoch()).count();
}

// ── task structure ────────────────────────────────────────
// every real time task has:
//   - period    (how often it runs)
//   - deadline  (max allowed time to complete)
//   - last_run  (when it last executed)
struct RTTask {
    const char* name;
    uint64_t period_us;    // how often to run (microseconds)
    uint64_t deadline_us;  // max execution time allowed
    uint64_t last_run;     // last execution timestamp
    bool deadline_missed;  // flag if deadline was missed
};

// ── watchdog timer ────────────────────────────────────────
// on real hardware watchdog resets the chip if not kicked
// prevents system from hanging forever
struct Watchdog {
    uint64_t timeout_us;
    uint64_t last_kick;
    bool triggered;
};

Watchdog watchdog = {50000, 0, false};  // 50ms timeout

void watchdog_kick() {
    watchdog.last_kick = get_time_us();
    watchdog.triggered = false;
}

void watchdog_check() {
    uint64_t now = get_time_us();
    if (now - watchdog.last_kick > watchdog.timeout_us) {
        watchdog.triggered = true;
        printf("WATCHDOG TRIGGERED! System would reset!\n");
    }
}

// ── simulate sensor read task ─────────────────────────────
// hard real time -- must complete in 1ms
void task_read_sensor(RTTask& task) {
    uint64_t start = get_time_us();

    // simulate sensor reading
    volatile int sensor_value = 0;
    for (int i = 0; i < 1000; i++) {
        sensor_value += i;  // simulate work
    }

    uint64_t elapsed = get_time_us() - start;

    // check deadline
    if (elapsed > task.deadline_us) {
        task.deadline_missed = true;
        printf("DEADLINE MISSED! %s took %lu us (limit %lu us)\n",
               task.name, elapsed, task.deadline_us);
    } else {
        printf("Task %-20s completed in %lu us (limit %lu us) OK\n",
               task.name, elapsed, task.deadline_us);
    }
}

// ── simulate control task ─────────────────────────────────
// hard real time -- must complete in 2ms
void task_control(RTTask& task) {
    uint64_t start = get_time_us();

    // simulate control calculation
    volatile double result = 0.0;
    for (int i = 0; i < 5000; i++) {
        result += i * 0.001;  // simulate work
    }

    uint64_t elapsed = get_time_us() - start;

    if (elapsed > task.deadline_us) {
        task.deadline_missed = true;
        printf("DEADLINE MISSED! %s took %lu us (limit %lu us)\n",
               task.name, elapsed, task.deadline_us);
    } else {
        printf("Task %-20s completed in %lu us (limit %lu us) OK\n",
               task.name, elapsed, task.deadline_us);
    }
}

// ── simulate communication task ───────────────────────────
// soft real time -- should complete in 5ms
void task_communicate(RTTask& task) {
    uint64_t start = get_time_us();

    // simulate UART transmission
    volatile int tx_bytes = 0;
    for (int i = 0; i < 10000; i++) {
        tx_bytes++;  // simulate work
    }

    uint64_t elapsed = get_time_us() - start;

    if (elapsed > task.deadline_us) {
        task.deadline_missed = true;
        printf("DEADLINE MISSED! %s took %lu us (limit %lu us)\n",
               task.name, elapsed, task.deadline_us);
    } else {
        printf("Task %-20s completed in %lu us (limit %lu us) OK\n",
               task.name, elapsed, task.deadline_us);
    }
}

int main() {

    printf("=== REAL TIME SYSTEM SIMULATION ===\n\n");

    // ── define tasks ──────────────────────────────────────
    RTTask sensor_task    = {"read_sensor",  1000, 2000,  0, false};
    RTTask control_task   = {"control",      2000, 5000,  0, false};
    RTTask comms_task     = {"communicate",  5000, 10000, 0, false};

    // ── PART 1: normal operation ──────────────────────────
    printf("=== NORMAL OPERATION ===\n");

    watchdog_kick();  // start watchdog

    for (int cycle = 0; cycle < 3; cycle++) {
        printf("\nCycle %d:\n", cycle + 1);

        task_read_sensor(sensor_task);
        task_control(control_task);
        task_communicate(comms_task);

        watchdog_kick();  // kick watchdog each cycle
        watchdog_check();
    }

    // ── PART 2: deadline miss simulation ─────────────────
    printf("\n=== DEADLINE STRESS TEST ===\n");

    // tighten deadlines to force misses
    RTTask tight_task = {"tight_deadline", 1000, 1, 0, false};

    for (int i = 0; i < 3; i++) {
        task_read_sensor(tight_task);
    }

    // ── PART 3: watchdog simulation ───────────────────────
    printf("\n=== WATCHDOG SIMULATION ===\n");

    watchdog_kick();
    printf("Watchdog started\n");

    // simulate system hang -- don't kick watchdog
    printf("Simulating system hang...\n");
    uint64_t hang_start = get_time_us();

    // busy wait to simulate hang
    while (get_time_us() - hang_start < 60000) {
        // not kicking watchdog!
        watchdog_check();
        if (watchdog.triggered) break;
    }

    // ── PART 4: timing summary ────────────────────────────
    printf("\n=== TIMING SUMMARY ===\n");
    printf("Sensor task deadline missed   : %s\n",
           sensor_task.deadline_missed  ? "YES" : "NO");
    printf("Control task deadline missed  : %s\n",
           control_task.deadline_missed ? "YES" : "NO");
    printf("Comms task deadline missed    : %s\n",
           comms_task.deadline_missed   ? "YES" : "NO");

    return 0;
}