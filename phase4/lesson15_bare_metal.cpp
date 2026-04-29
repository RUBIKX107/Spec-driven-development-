#include <cstdint>   // uint8_t, uint32_t etc
#include <cstring>   // memset, memcpy
#include <cstdio>    // printf (only basic IO allowed)

// ── simulate hardware registers ───────────────────────────
// on real embedded systems these are fixed memory addresses
// like: volatile uint32_t* LED = (uint32_t*)0x40020000;
// we simulate them as global variables

volatile uint32_t GPIO_PIN    = 0x00;  // GPIO state
volatile uint32_t TIMER_COUNT = 0x00;  // timer register
volatile uint32_t UART_TX     = 0x00;  // UART transmit

// ── simulate interrupt flag ───────────────────────────────
volatile bool timer_interrupt = false;

// ── bare metal memory pool ────────────────────────────────
// on embedded systems you often manage your own memory pool
// instead of using heap (new/delete is dangerous on bare metal)
#define MEMORY_POOL_SIZE 256
uint8_t memory_pool[MEMORY_POOL_SIZE];  // fixed size pool
int pool_index = 0;

// simple allocator -- no OS needed
void* bare_malloc(int size) {
    if (pool_index + size > MEMORY_POOL_SIZE) {
        return nullptr;  // out of memory!
    }
    void* ptr = &memory_pool[pool_index];
    pool_index += size;
    return ptr;
}

// ── simulate GPIO (General Purpose Input Output) ──────────
// on real hardware this controls pins
// like turning LED on/off
void gpio_set_high(int pin) {
    GPIO_PIN |= (1 << pin);   // set bit
    printf("GPIO pin %d set HIGH\n", pin);
}

void gpio_set_low(int pin) {
    GPIO_PIN &= ~(1 << pin);  // clear bit
    printf("GPIO pin %d set LOW\n", pin);
}

bool gpio_read(int pin) {
    return (GPIO_PIN >> pin) & 1;
}

// ── simulate timer ────────────────────────────────────────
void timer_tick() {
    TIMER_COUNT++;
    if (TIMER_COUNT % 10 == 0) {
        timer_interrupt = true;  // fire interrupt every 10 ticks
    }
}

// ── interrupt handler ─────────────────────────────────────
// on real hardware this runs automatically when interrupt fires
void timer_interrupt_handler() {
    printf("INTERRUPT! Timer count = %d\n", TIMER_COUNT);
    timer_interrupt = false;  // clear interrupt flag
}

// ── simulate UART (serial communication) ──────────────────
void uart_send(const char* message) {
    UART_TX = 1;  // TX line high = transmitting
    printf("UART TX: %s\n", message);
    UART_TX = 0;  // TX line low = done
}

int main() {

    // ── PART 1: bare metal memory ─────────────────────────
    printf("=== BARE METAL MEMORY ===\n");
    printf("Memory pool size: %d bytes\n", MEMORY_POOL_SIZE);

    // allocate from our custom pool
    int* sensor_data = (int*)bare_malloc(4 * sizeof(int));
    if (sensor_data == nullptr) {
        printf("Out of memory!\n");
        return 1;
    }

    // fill sensor data
    sensor_data[0] = 23;  // temperature
    sensor_data[1] = 65;  // humidity
    sensor_data[2] = 100; // pressure
    sensor_data[3] = 42;  // light

    printf("Sensor readings:\n");
    printf("  Temperature : %d C\n",  sensor_data[0]);
    printf("  Humidity    : %d %%\n", sensor_data[1]);
    printf("  Pressure    : %d Pa\n", sensor_data[2]);
    printf("  Light       : %d lx\n", sensor_data[3]);

    // ── PART 2: GPIO simulation ───────────────────────────
    printf("\n=== GPIO SIMULATION ===\n");

    gpio_set_high(0);   // turn on LED pin 0
    gpio_set_high(1);   // turn on LED pin 1
    printf("GPIO state: 0x%02X\n", GPIO_PIN);

    gpio_set_low(0);    // turn off LED pin 0
    printf("GPIO state: 0x%02X\n", GPIO_PIN);
    printf("Pin 1 is: %s\n", gpio_read(1) ? "HIGH" : "LOW");

    // ── PART 3: timer and interrupts ─────────────────────
    printf("\n=== TIMER AND INTERRUPTS ===\n");

    // simulate main loop -- on embedded this runs forever
    for (int i = 0; i < 25; i++) {
        timer_tick();

        // check interrupt flag -- like embedded main loop
        if (timer_interrupt) {
            timer_interrupt_handler();
        }
    }

    // ── PART 4: UART communication ────────────────────────
    printf("\n=== UART COMMUNICATION ===\n");
    uart_send("Hello from embedded device!");
    uart_send("Temperature: 23C");
    uart_send("Status: OK");

    printf("\nBare metal program complete!\n");
    return 0;
}