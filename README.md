# ctock-seer
ctock Seer (pronounced stock seer) is a example program written in c++, hence why the name starts with an lowercase "c," meant to find the best way to predict stocks using machine learning. It is used by me for my undergraduate research at Queensborough Community College. It is based on a research article in [Soft Computing Letters](https://www.sciencedirect.com/journal/soft-computing-letters/) published by [Elsevier](https://www.elsevier.com/). The name of the article is "[Forecasting and optimization stock predictions: Varying asset profile, time window, and hyperparameter factors](https://www.sciencedirect.com/science/article/pii/S2772941923000054)" by Alzaman, Chaher.

## Dependencies
ctock Seer uses *yfinance* to grab the stock data from Yahoo Finance's API. However, *yfinance* is a Python library. *pybind11* is then used to convert the Python database to c++ data types.

Dependencies needed:

- cmake (to build)
- Python
- pybind11

The below are Python libraries. They can be install with something like *pip*.

- yfinance and its dependencies
	- beautifulsoup4
	- curl_cffi
	- frozendict
	- multitasking
	- numpy
	- pandas
	- peewee
	- platformdirs
	- protobuf
	- pytz
	- requests
	- websockets

## How to Build
If you are in a Linux environment, run *cmake* in the build folder as shown below.

To make and enter the build directory

	mkdir build
	cd build/

To build the binary

	cmake ../
	cmake --build ./

To run the binary

	./ctock-seer

If you are in a Windows or macOS environment, good luck.

Okay, I think you can just download *cmake* and run the commands above the same way, but I do not know.