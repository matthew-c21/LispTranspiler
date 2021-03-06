#ifndef LISP_STDLISP_H
#define LISP_STDLISP_H

#include "data.h"

// TODO(matthew-c21): I need to figure out a proper means of error handling.
// TODO(matthew-c21): To go with prior TODO, determine behavior of division by 0.

/** Function pointer specifically designed to manage LISPy calling conventions.  */
typedef struct LispDatum* (*LispFunction)(struct LispDatum**, uint32_t);

/**
 * Sum all values provided.
 *
 * If no arguments are supplied, return 0. If non-numeric arguments are supplied, return NULL.
 */
struct LispDatum* add(struct LispDatum** args, uint32_t nargs);

/**
 * Subtract the 2nd, 3rd, etc., arguments from the first.
 *
 * If no arguments are supplied, return 0. If one argument is supplied, return the same value. If non-numeric arguments
 * are supplied, return NULL.
 */
struct LispDatum* subtract(struct LispDatum** args, uint32_t nargs);

/**
 * Multiply all provided arguments.
 *
 * If no arguments are supplied, return 1. If non-numeric arguments are supplied, return NULL.
 */
struct LispDatum* multiply(struct LispDatum** args, uint32_t nargs);

/**
 * Divide the first argument by all subsequent arguments.
 *
 * If no arguments are supplied, return 0. If only one argument is supplied, return the same value. If non-numeric
 * arguments are supplied, return NULL.
 */
struct LispDatum* divide(struct LispDatum** args, uint32_t nargs);

/**
 * Given integers a and b, return the smallest integer m such that a = b(mod m).
 *
 * Takes exactly two integer arguments. If anything else is provided, return NULL.
 */
struct LispDatum* mod(struct LispDatum** args, uint32_t nargs);

/**
 * Given integers a and b, return a nil terminated list containing two numbers x and y such that y<a and a = bx + y.
 *
 * Takes exactly two integer arguments. If anything else is provided, return NULL.
 */
struct LispDatum* division(struct LispDatum** args, uint32_t nargs);

struct LispDatum* format(struct LispDatum** args, uint32_t nargs);

void display(struct LispDatum* datum);

int datum_cmp(const struct LispDatum* a, const struct LispDatum* b);

/**
 * Determines if two objects are strictly equal.
 *
 * This is equivalent to the eqv? predicate found in Scheme. See the R7RS spec for more information.
 */
struct LispDatum* eqv(struct LispDatum** args, uint32_t nargs);

// Comparative functions and logical manipulation
// TODO(matthew-c21): Most comparators can be discarded once user generated functions are in order.
struct LispDatum* less_than(struct LispDatum** args, uint32_t nargs);
struct LispDatum* num_equals(struct LispDatum** args, uint32_t nargs);
struct LispDatum* greater_than(struct LispDatum** args, uint32_t nargs);
struct LispDatum* less_than_eql(struct LispDatum** args, uint32_t nargs);
struct LispDatum* greater_than_eql(struct LispDatum** args, uint32_t nargs);

/**
 * Returns last value in a list
 * @param args
 * @param nargs
 * @return
 */
struct LispDatum* logical_and(struct LispDatum** args, uint32_t nargs);
struct LispDatum* logical_or(struct LispDatum** args, uint32_t nargs);
struct LispDatum* logical_not(struct LispDatum** args, uint32_t nargs);

// LIST FUNCTIONS

/**
 * Obtain the first element of a list. Fails if the argument is not a list, or if it is empty as the first element of an
 * empty list is not defined.
 */
struct LispDatum* car(struct LispDatum** args, uint32_t nargs);

/**
 * Obtain the linked child nodes in a list. When used on an empty list or nil, returns an empty list. When used on an
 * improper list, e.g. `(cdr '(a . b))`, returns the second item - in this case the symbol 'b'.
 */
struct LispDatum* cdr(struct LispDatum** args, uint32_t nargs);

/**
 * Obtains the length of a proper list. Fails on non-list arguments, or when receiving too many arguments at once.
 * @throws Type error if given non-proper list arguments.
 * @throws Argument error if not given exactly one argument
 */
struct LispDatum* length(struct LispDatum** args, uint32_t nargs);
struct LispDatum* cons(struct LispDatum** args, uint32_t nargs);

/**
 * Creates a linked list structure using the provided arguments. If `nargs` is 0, then the args array is not evaluated
 * at all, meaning that `list(NULL, 0)` always returns a valid, empty list.
 * @return
 */
struct LispDatum* list(struct LispDatum** args, uint32_t nargs);

/**
 * Combines multiple lists together. Only joins top level lists. Returns an empty list if no lists are provided. Returns
 * the given list if only one list is provided. This has an interesting quirk that `(append nil)` returns nil instead of
 * an empty list. It will fail if given a single argument of any other type, however.
 *
 * Example: (append (1 2) (3 4)) ==> (1 2 3 4)
 */
struct LispDatum* append(struct LispDatum** args, uint32_t nargs);

/**
 * Constructs a list in reverse order. This items referred to in the new reversed list are the same as in the
 * non-reversed list. For a list too short to reverse (nil, empty, single element), the original argument is returned.
 *
 * @throws Argument exception if anything other than one argument is passed.
 * @throws Type exception if the argument is neither a proper list nor nil.
 */
struct LispDatum* reverse(struct LispDatum** args, uint32_t nargs);

#endif //LISP_STDLISP_H
