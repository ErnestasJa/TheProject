cmake_minimum_required (VERSION 2.6)

include_directories(./)
add_library (sigc++
sigc++/connection.cc
sigc++/signal_base.cc
sigc++/signal.cc
sigc++/trackable.cc
sigc++/functors/slot_base.cc
sigc++/functors/slot.cc
sigc++/adaptors/lambda/lambda.cc)
