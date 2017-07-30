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
