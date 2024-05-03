# const and constexpr

While const and constexpr serve similar purposes of declaring constants, constexpr offers additional benefits in scenarios where compile-time evaluation is required or beneficial. Here's an example to illustrate this:

```
#include <iostream>

// Function to compute factorial at compile time using constexpr
constexpr int factorial(int n) {
    return (n <= 1) ? 1 : (n * factorial(n - 1));
}

int main() {
    // Using constexpr for compile-time evaluation
    constexpr int fact5 = factorial(5); // factorial(5) is computed at compile time
    std::cout << "Factorial of 5 (computed at compile time): " << fact5 << std::endl;

    // Using const for runtime value
    const int num = 5;
    const int factRuntime = factorial(num); // factorial(num) is computed at runtime
    std::cout << "Factorial of " << num << " (computed at runtime): " << factRuntime << std::endl;

    return 0;
}
```

In this example:

The factorial function is declared as constexpr, allowing it to be evaluated at compile time when its argument is known at compile time.
We use constexpr to compute factorial(5) at compile time, and the result is stored in fact5.
We also use const to compute factorial(num) at runtime, where num is a variable with a value known only at runtime.
The benefit of using constexpr here is that it allows the compiler to compute the factorial of a known value (5) at compile time, which can lead to optimizations such as constant folding and potentially faster program execution. This is not possible with const, which is evaluated at runtime.

So, while const is sufficient for declaring runtime constants, constexpr provides additional benefits for compile-time evaluation, enabling optimizations and ensuring certain computations are performed at compile time when possible.