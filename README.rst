Devistator Backtesting Platform
=====
I built this backtest platform after building a few in Matlab and Python.  I chose C++ because:
 * it's fast 
 * I can trade in C++
The second point may not seem relevant but once you develop a strategy that looks good in backtesting you will want to start to paper trade it.  If papertrading goes bad, then you should ask "why is my trading different from my backtesting?" If you trade in one language but backtested in another language you ported things over, and in doing so you may have made errors.  

Features 
============
A good backtest platform does it's best to remove unwanted bias, some forms of bias cannot be avoided in the code.  Look-ahead bias can be avoided with proper coding.  


Execution
============
This default main.cpp I have opens a bunch of text files, parses them, and then runs a given strategy on the historical data.  To execute this after you have built the code enter:
``../Momentum 2.0``
It is assumed that you are in a directory with a bunch of text files containing historical data.  
