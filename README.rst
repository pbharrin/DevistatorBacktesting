Devistator Backtesting Platform
=====
I built this backtesting platform after outgrowing platforms in Matlab and Python.  I chose C++ because:
 * it's fast 
 * I can trade in C++
It is very fast.  On a two year old machine this code rips through two years of one minute data in less than a second.  You can make some mistakes with C++ and kill its perfomance.  Execptions are a good example.  Care was taken to avoid the performance killing features of C++, and keep the code closer to pure C.  
The second point may not seem relevant but once you develop a strategy that looks good in backtesting you will want to start to paper trade it.  If papertrading goes bad, then you should ask "why is my trading different from my backtesting?" If you trade in one language but backtested in another language you ported things over, and in doing so you may have made errors.  

Features 
============
A good backtest platform does it's best to remove unwanted bias, some forms of bias cannot be avoided in the code.  Look-ahead bias can be avoided with proper coding.  This code demonstrates how to create a strategy class and then evaluate the strategy.  Bars are fed to the strategy class, this prevents the strategy class from having access to future price movement, therby removing look ahead bias.  
The stratEval function calculates a number of performance metrics for the given strategy, and the given historical data.  The following are displayed:
 * Sharpe's Ratio
 * Max Draw Down
 * Number of Trades
 * Percentage Time in the Market
 * Returns After Transaction Costs 

The Strategy Class
============
In order to make this as modular as possible, an abstract class: Strategy was created (see Strategy.h).  This does nothing and never gets executed.  However all real strategies will inherit from this class.  Every class that inherts from this must implement the execute() method, which requires a timeSlice as input.  This allows the use of any time scale (tick data is ok).  
The idea here is that you write different strategies, and easily swap them in and out.  The strategy can do anything you want, maintain state etc.  The strategy cannot see future bars, and this prevents look-ahead bias.   If you want to look at multiple securities at one time, you can do that by redefining histTextLoad.cpp.  This allows things like trading baskets of securities for stat. arb.  
One example strategy class MomentumStrat2 is included, with transaction costs included this is wildly unprofitable.  (If it were profitable I wouldn't be posting it.)    

Execution
============
This default main.cpp I have opens a bunch of text files, parses them, and then runs a given strategy on the historical data.  To execute this after you have built the code enter:
``../Momentum 2.0``
It is assumed that you are in a directory with a bunch of text files containing historical data.  
