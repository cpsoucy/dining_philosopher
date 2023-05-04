#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>
#include <random>
using namespace std;

//# philosophers;
const int NUM_PHILOSOPHERS = 5;

//all possible states that philosophers can be in at any given time:
enum State {
  THINKING,
  HUNGRY,
  EATING,
    DEAD
};

mutex forks[NUM_PHILOSOPHERS];
State philosopherStates[NUM_PHILOSOPHERS];
int philosopherHungerMeters[NUM_PHILOSOPHERS];

void philosopher(int id) {
  philosopherHungerMeters[id] = 0;
  mt19937 rng(id);
  uniform_int_distribution<int> dist(100, 200);

  while (philosopherStates[id] != DEAD) {
    // philosopher is thinking:
    philosopherStates[id] = THINKING;
    if (philosopherHungerMeters[id] >= 10) {
      cout << "Philosopher " << id << " has died." << endl;
      exit(0);
    }
    cout << "Philosopher " << id << " is thinking. Hunger meter: " << philosopherHungerMeters[id] << en\
dl;
    this_thread::sleep_for(chrono::milliseconds(dist(rng)));
    philosopherHungerMeters[id] += 1;

    //philosopher is aquiring forks:
    philosopherStates[id] = HUNGRY;
    cout << "Philosopher " << id << " is hungry. Hunger meter: " << philosopherHungerMeters[id] << endl\
;
    bool hasLeftFork = false;
    while (!hasLeftFork) {
      forks[id].lock();
      if (!forks[(id + 1) % NUM_PHILOSOPHERS].try_lock()) {
        forks[id].unlock();
        this_thread::sleep_for(chrono::milliseconds(dist(rng)));
      } else {
        hasLeftFork = true;
      }
    }
    philosopherHungerMeters[id] -= 1;

    philosopherStates[id] = EATING;

    //philosopher is able to eat after aquiring forks:
    cout << "Philosopher " << id << " is eating. Hunger meter: " << philosopherHungerMeters[id] << endl\
;
    this_thread::sleep_for(chrono::milliseconds(dist(rng)));
    philosopherStates[id] = THINKING;

    //philosopher is putting down forks after eating, so other philosophers can aquire forks:
    forks[id].unlock();
    forks[(id + 1) % NUM_PHILOSOPHERS].unlock();
  }
}
int main() {
  thread philosophers[NUM_PHILOSOPHERS];

  // Initialize philosopher states:
  for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
    philosopherStates[i] = THINKING;
    philosopherHungerMeters[i] = 0;
  }

  // Create philosopher threads:
  for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
    philosophers[i] = thread(philosopher, i);
  }

  // Wait for philosopher threads to finish:
  for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
    philosophers[i].join();
  }

  return 0;
}
