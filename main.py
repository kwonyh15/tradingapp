import pandas
import loadData
import loadHistory
import matplotlib.pyplot as plt

# Single Data
# loadData.pull_historical_data("066570.KS+AAPL+GOOG+MSFT&f=snd1l1yr")/

# stock_data = pandas.read_csv("/Users/mac/Desktop/S&P/066570.KS+AAPL+GOOG+MSFT&f=snd1l1yr.csv",
#                              names=['symbol', 'name', 'last trade date', 'last trade','dividend yeild','P/E ratio'])

# -------------------------------

# History Data
loadHistory.pull_historical_data("s=066570.KS&a=04&b=06&c=2014&d=04&e=06&f=2016&g=d&ignore=.csv")

stock_data = pandas.read_csv("/Users/mac/Desktop/S&P/s=066570.KS&a=04&b=06&c=2014&d=04&e=06&f=2016&g=d&ignore=.csv")


print(stock_data)

plt.plot(stock_data.index, stock_data['Open'])
plt.show()
