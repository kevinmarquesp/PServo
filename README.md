# PServo

## Installation

<!--TODO-->


## Usage

<!--TODO-->


## Contributing

```bash
git clone https://github.com/kevinmarquesp/PServo
cd PServo

make deps # Clone and/or build vendorized libraries.
```


### Testing (Arduino Board not Required)

#### Run All Tests

```bash
make deps
make test
```


#### Run a Specific Test

```bash
make deps
make test GTEST_SRCS=extra/gtest/test_gtest.cpp
```


### Todos

* [ ] Include the current state on the `PServo::get_props()` method.
  * [ ] Update the `Defaults` test cases to include the new *props* values.
* [ ] Use a `unsigned long *const` (16 bits) for the *timer* property.
  * [ ] Update the `Defaults` test cases to use the correct constructor.
  * [ ] Update the `MValues` test cases to use construct the timer properly.
