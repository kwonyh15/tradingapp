from urllib.request import urlretrieve, ContentTooShortError

base_url = "http://finance.yahoo.com/d/quotes.csv?s="


def make_url(ticker_symbol):
    return base_url + ticker_symbol


output_path = "/Users/mac/Desktop"


def make_filename(ticker_symbol, directory="S&P"):
    return output_path + "/" + directory + "/" + ticker_symbol + ".csv"


def pull_historical_data(ticker_symbol, directory="S&P"):
    try:
        urlretrieve(make_url(ticker_symbol), make_filename(ticker_symbol, directory))
    except ContentTooShortError as e:
        print("entered exception code")
        outfile = open(make_filename(ticker_symbol, directory), "w")
        outfile.write(e.content)
        outfile.close()
