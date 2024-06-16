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
make test GTEST_UNITS=extra/gtest/test_gtest.cpp
```
