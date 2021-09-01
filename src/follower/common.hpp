#ifndef COMMON_HPP_
#define COMMON_HPP_

#include <map>
#include <utility>


enum Metric {FREE_CPU, FREE_MEMORY, FREE_DISK, LATENCY, BANDWIDTH, CONNECTED_IOTS, BATTERY, TOTAL_MEMORY, TOTAL_DISK, LIGHT_INTENSITY};
enum State {STABLE, UNSTABLE, INCREASING, DECREASING, TOO_HIGH, TOO_LOW, ALARMING_HIGH, ALARMING_LOW, OK, NONE};

const std::map<std::string, Metric> lMetrics = {{"free_cpu", Metric::FREE_CPU}, 
                                                {"free_memory", Metric::FREE_MEMORY}, 
                                                {"free_disk", Metric::FREE_DISK},
                                                {"latency", Metric::LATENCY},
                                                {"bandwidth", Metric::BANDWIDTH},
                                                {"connected_iots", Metric::CONNECTED_IOTS},
                                                {"battery", Metric::BATTERY},
                                                {"total_memory", Metric::TOTAL_MEMORY},
                                                {"total_disk", Metric::TOTAL_DISK},
                                                {"light_intensity", Metric::LIGHT_INTENSITY}};

const std::map<std::string, State> lStates = {{"stable", State::STABLE},
                                                  {"unstable", State::UNSTABLE},
                                                  {"increasing", State::INCREASING},
                                                  {"decreasing", State::DECREASING},
                                                  {"alarming_high", State::ALARMING_HIGH},
                                                  {"alarming_low", State::ALARMING_LOW},
                                                  {"too_high", State::TOO_HIGH},
                                                  {"too_low", State::TOO_LOW},
                                                  {"ok", State::OK}};

#endif