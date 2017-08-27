# MegiddoLP
An implementation of Megiddo's linear programming (LP) algorithm

## The problem

The goal of LP is to optimize some objective function, given some constraints.
It is used to solve a variety of problems that can be modeled in terms of,
e.g., demand given some finite resource supply.

Optimal objective values occur along the boundary of the feasible region, which
is the space bounded on all sides by the constraints, in the direction of their
inequalities (if such a region exists).  If a feasible solution exists then
either the optimum lies at the intersection of two constraints or there are
many optima all of which lie along a single constraint.

Megiddo's algorithm relies on discarding a constant fraction of the constraints
based off of the knowledge that they fall outside the feasible region in the
direction of the optimum.

## The algorithm

For the 2D Megiddo algorithm, the first step is to rotate the objective
function, which is a vector, and the constraints, which are all linear, such
that the objective function points downward.  From there, the constraints can
be partitioned into a set whose inequalities point upwards and a set whose
inequalities point downwards.  Those constraints that are innermost to the
feasible region form what are referred to as its convex and concave
"envelopes".

Next, arbitrarily pair up all constraints from one (upward or downward) set and
calculate their intersections, all in linear time.  If a pair is parallel,
discard the outer constraint and ignore this pair.  Find the median of the
intersections by x-coordinate in linear time using deterministic median
finding.  Find the y-values for all constraints of both sets for this
x-coordinate in linear time.  If the minimum concave constraint is below the
maximum convex constraint, the optimum lies in the direction of their
intersection, unless they are parallel, in which case the solution is
infeasible.  If the minimum concave constraint is *above* the maximum convex
one, this x-coordinate is *a* feasible solution (not necessarily the optimum),
and the optimum lies in the opposite direction of their intersection.

Back to the pairs.  Say the optimum lies to the left of the midpoint of the
intersections.  That means the outer constraint of each pair to the right of
the midpoint is shadowed by the inner constraint in the direction of the
optimum.  As such, it can be discarded; a quarter of that set can be discarded
in total.  Then, choose the other set, pair, etc.  Repeat until the optimum
is found or the solution is deemed infeasible.

Because a constant fraction of the constraints can be discarded in each
iteration, this runs in linear time overall.

## Comparing Clp and Megiddo

It is useful to consider how MegiddoLP stacks up against another off-the-shelf
LP solver, Coin-Or Clp.  MegiddoLP includes a wrapper that conforms to a subset
of Clp's interface, so this is easy to compare given some driver.

For this test, ten feasible linear programs were randomly generated, each with
ten, twenty or thirty constraints.  Most linear programs are infeasible, and as
such this search for feasible programs is in an exponentially large space.
Because direct random generation of larger feasible programs is intractable,
the ten programs with thirty constraints were randomly permuted to generate
larger, slightly different programs.  These were then much more quickly
searched for feasible larger programs.  Unfortunately, this had an upper limit
imposed by the compiler: program sets with more than 9000 constraints were
unable to compile.

The user and sys times given below in the format *mean ± SD* are the average
output over multiple runs as reported by the Linux `time` utility to run all
ten programs in each set, along with any program overhead.

|   Algorithm   |  Constraints  |       User       |        Sys       |
| ------------- | ------------- | ---------------- | ---------------- |
| Clp (Simplex) |        10     | 0.3608s ± 0.0142 | 0.1048s ± 0.0078 |
| MegiddoLP     |        10     | 0.3536s ± 0.0192 | 0.1088s ± 0.0047 |
| Clp           |        20     | 0.3600s ± 0.0095 | 0.1112s ± 0.0122 |
| MegiddoLP     |        20     | 0.3488s ± 0.0073 | 0.1064s ± 0.0106 |
| Clp           |        30     | 0.3640s ± 0.0277 | 0.1096s ± 0.0070 |
| MegiddoLP     |        30     | 0.3624s ± 0.0120 | 0.1120s ± 0.0131 |
| Clp           |        60     | 0.3688s ± 0.0117 | 0.1008s ± 0.0170 |
| MegiddoLP     |        60     | 0.3624s ± 0.0153 | 0.1048s ± 0.0064 |
| Clp           |       300     | 0.3592s ± 0.0260 | 0.1144s ± 0.0120 |
| MegiddoLP     |       300     | 0.3544s ± 0.0192 | 0.1240s ± 0.0139 |
| Clp           |      3000     | 0.3936s ± 0.0155 | 0.1256s ± 0.0070 |
| MegiddoLP     |      3000     | 0.5368s ± 0.0142 | 0.1024s ± 0.0176 |
| Clp           |      6000     | 0.5072s ± 0.0237 | 0.1120s ± 0.0044 |
| MegiddoLP     |      6000     | 0.7112s ± 0.0220 | 0.1040s ± 0.0141 |
| Clp           |      9000     | 0.6592s ± 0.0167 | 0.1080s ± 0.0121 |
| MegiddoLP     |      9000     | 0.8944s ± 0.0140 | 0.1112s ± 0.0188 |

It's clear that for small programs, the time to solve all ten problems with
either algorithm is negligible as compared to the overhead.  The sys time is
basically independent of the program size, which is also to be expected.
Taking these runtimes as a baseline, as more constraints are added the runtime
of MegiddoLP appears to increase linearly at the rate of about 180ms per 3000
constraints, or about 60μs per constraint.  On the other hand, Clp is more
efficient for the programs tested but the increase in runtime in the test
programs is superlinear.  If this trend continues with even larger programs
then it's possible that MegiddoLP could overtake Clp for programs with more
than around 20000 constraints.

## Caveats

MegiddoLP has a few quirks.  As with any algorithm using IEEE floating point
numbers, precision is limited by the representability of the values.  Because
this implementation follows the algorithm presented in Megiddo's paper, it
performs some unnecessary steps such as rotating the constraints to point
downwards, which introduces minor error in the lowest bits of the significand.
When the optimum is at zero, this sometimes causes it to be at some epsilon
away from zero; it is impossible to be certain within MegiddoLP whether or not
this should be zero, so the optimum should be taken in the context of the
problem being solved.

MegiddoLP also does not attempt to decide if the optimum is unbounded.  In
these cases, the solution will just be enormous, approaching or slightly
exceeding the maximum 32-bit float.

Finally, although many linear programs have been tested using MegiddoLP (with
many bugs discovered along the way), it is not bug free; a small number of
programs return a different optimum from Clp; because these programs were
randomly generated, it is incredibly unlikely that there are actually multiple
solutions.  However, the large number of constraints involved make it
increasingly difficult to root cause.
