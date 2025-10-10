import yfinance as yf
import pandas as pd
import sys

df = yf.Ticker("AAPL").history(start="2022-01-01", end="2024-12-31")
df.drop(index=df.index[0], axis=0, inplace=True)
print(df)
#print(df.loc['2023-01-03 00:00:00-05:00']['Volume'])
#print(type(df.loc['2023-01-03 00:00:00-05:00']['Volume']))
#print(sys.modules)
#print(sys.modules[__name__])
#print(type(sys.modules))
#print(type(sys.modules[__name__]))
gfg_csv_data = df.to_csv("example.csv", index = True)
print("\nCSV String:\n", gfg_csv_data)
