# Coding style

This is a short document that describes my preferred coding style on many of
my personal C projects. Much like Linus Torvalds, I won't force my views on
anyone but please take note of some of the points made here.

## Indentation

Don't use tabs, use 8 spaces. Modern editors such as vim or emacs can handle
this quite easily. Why so large? Because larger indentations make the code much
easier to look at and parse with human eyes; it makes mistakes much more
glaringly obvious. Large indents have the added benefit of warning when
functions are nested too deep.

Case labels should be indented past their parent switch statement, e.g.:

        switch (suffix) {
                case 'G':
                        foo();
                        break;
                case 'g':
                        bar();
                        break;
                default:
                        break;
        }

Don't put multiple statements on a single line (it makes it look like you're
trying to hide something!)

        if (foo) conditional_func();
                always_done();

Don't leave whitespace at the end of lines.


## Breaking long lines

The column limit is (in general) 80 characters. Statements longer than 80
characters should be broken into more readable chunks. The exception to this is
strings; don't break these as it hinders the ability to grep for them.

## Braces and spaces

Braces should be done as it was by K&R C, e.g.:

        if (foo) {
                bar();
        }

This also applies to switch statements, while and for loops, and, yes, even
functions.

The closing brace, if followed by a continuation of the same statement, should
be placed on a line of its own, e.g.:

        if (foo) {
                ..
        } else {
                ..
        }

Furthermore, braces should not be used when there is only a single statement.

        if (foo)
                bar();

The exception to this rule is if only one branch is itself a single statemnt.

        if (fizz) {
                statement0();
                statement1();
        } else if (buzz) {
                single_statement();
        }

Use a space after these keywords::
        
        if switch case for do while

But not with functions or function-like statements:

        x = sizeof(struct thing);

Do not add spaces inside parenthesized expressions:

        // BAD
        s = sizeof( struct bar );

When declaring a pointer, the asterisk should be adjacent to the variable not
the type.

        char *buffer;

If the pointer is in a function return, the opposite is true:

        struct bar* init_bar(void) {
                ..
        }

Use one space around most binary and ternary operators, such as::

        = + - < > * / % | & ^ <= >= == != ? :

But no space after unary operators::

        & * + - ~ ! sizeof

No space before or after ``++`` or ``--``, and no space around ``.`` or ``->``.

Do not leave trailing whitespace at the ends of lines or files.

## Naming

Since C is a terse language, name conventions should be as well. Variables
should not have verbose names, i.e., instead of ``TemporaryPointerToInt``, ``tmp``
is just less descriptive and carries the same intent. The exception to this is
obviously globals: global variables and functions need to be descriptive.

Encoding the type name into the name of the variable or function is frowned
upon, as this just confuses the programmer.

## Typedefs

Using constructs like 'typename_t' is heavily frowned upon, because this removes
the backing type information. Data structures, enums and pointers should not be
hidden behind a typedef.

Most of the time, use of a typedef should be avoided unless you're using
totally opaque objects, or if you're using clear integer types such as::

        typedef long int u32;

Most importantly, objects should not be behind more than a single typedef. For
instance, things like this::

        typedef long int u32;
        ...
        some_other_code();
        ...
        typedef u32 uint32_t;

should be avoided.

## Data structures

Data structures that are *readable* by more than one thread must have a
reference count. If it is *writable* by more than one thread, it must have a
lock. These are not mutually exclusive and are usually employed together.

## Functions returns and names

Functions can exit in a multitude of different ways, and one of the most common
is a value indicating success or failure. This can be represented with an
error-code or a boolean value. Mixing these two can lead to problematic bugs.
To prevent bugs like this, follow this convention::

        If the name of a function is an action or imperative command,
        the function should return an error-code integer. If the name
        is a predicate, the function should return a "succeeded" boolean.

For instance, ``add_ints()`` should return 0 or -1, and ``list_empty()``
should return 0 or 1.

Functions that create or destroy a data structure should just return a pointer,
or ``NULL`` if error, or void respectively.

## Using ``goto``

Use of ``goto`` is often encouraged in *certain* situations. Most of the time,
if you think you need a ``goto`` statement, go back over your code just to be
sure. However, if you think you need a ``goto`` statement for readability or to
exit a deeply nested loop, use of a ``goto`` is preferred. For instance, a
function might return in several places but some common work might need to be
done. In cases like these, a ``goto`` can be immensely helpful.

Choose label names that describe why the ``goto`` even exists.

        int foo(int x) {
                int ret = 0;
                char *buffer = malloc(x);
                if (!buffer)
                        return -1;
        
                if (condition0) {
                        while (loop_var) {
                                ...
                        }
                        ret = 1;
                        goto cleanup;
                }
                ...
        cleanup:
                free(buffer);
                return ret;
        }

## Commenting

Comments are good, but there is a danger of over-commenting. Don't try to
explain *how* your code works in a comment. It's much better to write the code
so that the *working* is obvious.

Generally, you want your comments to tell *what* your code does, not *how*.
Functions should generally not have comments inside the function body. That is
a sign of an over-engineered function that should be broken into helper
functions. You should place comments at the head of the function, telling people
what it does, and possibly *why* it does it.

When commenting functions, please use the format specified at :ref:`doc_guide`.

Multiline comments should look like the following:

        /*
         * Multiline comment
         * Please use this style of commenting consistently.
         */

## Macros and enums

All macros should be capitalized.

        #define PI 3.1415
        #define FOO(int var) bar(var, 13)

Macros with multiple statements should be enclosed in a do - while block:

        #define MACRO(arg0, arg1) \
                do { \
                        if (arg0 == arg1) \
                                call(arg0, arg1) \
                } while (0)

This makes the macro uniform when calling it alongside other functions.

Don't affect control flow in a macro:

        #define FOO(x) \
                do { \
                        if (blah(x) < 0) \
                                return -1; \
                } while (0)

Don't have magic values in macros:

        #define FOO(val) bar(index, val)

Don't have arguments that can be used as l-values.

Remember operator precedence; always enclose expressions in parentheses:

        #define PI 3.1415
        #define CIR (PI * 10)

Don't use common local variable names in macros:

        #define FOO(x) \
        ({ \
                ret = sizeof(int); \
        })

Enums are preferred when defining several related constants.

Macros that behave like functions should not be capitalized and should be
treated as if they were functions; however, an inlined function should always
be preferred over this. This side-steps many of the problems addressed here.

## Printing messages

User-visible output should have correct spelling and mostly correct grammar.
These messages do not have to end in a period. Make these messages concise and
unambiguous.


## Inline assembly

Inline assembly is a thing of the past. Don't use it.

## References

* The Linux Kernel Coding Style - Linus Torvalds.
* The C Programming Language - Brian W. Kernighan, Dennis M. Ritchie
