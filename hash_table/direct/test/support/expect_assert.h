// Ceedling test support for expecting assert() failures.

#ifndef ASSERT_H
#define ASSERT_H

// Expect an assert() failure.
void expect_assert(void);

// Clear the expect for an assert() failure.
void expect_assert_clear(void);

#endif
