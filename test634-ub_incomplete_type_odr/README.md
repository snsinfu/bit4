`hash(trigger)` triggers undefeind behavior and evaluates to different values in
different translation units. The issue goes if we actually define the incomplete
struct type in the header.
