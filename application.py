import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions
from werkzeug.security import check_password_hash, generate_password_hash
from datetime import datetime

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""

    # Obtain all user's inventory
    query = db.execute("SELECT * FROM inventory \
                       JOIN symbols ON inventory.symbolID = symbols.symbolID \
                       WHERE userID = :userID",
                       userID=session['user_id'])

    # Obtain details for each company
    total_balance = 0.0
    companies = []
    for company in query:
        # Create dict to hold details
        company_details = {}

        # Lookup current pricing and name
        quote = lookup(company['symbol'])
        if quote is None:
            # If couldn't be found set name and price to empty
            quote = {'name': '', 'price': None, 'symbol': company['symbol']}

        company_details['name'] = quote['name']
        company_details['symbol'] = quote['symbol']
        company_details['price'] = quote['price']
        company_details['shares'] = company['shares']
        company_details['holding'] = None if (quote['price'] is None) else (company['shares'] * quote['price'])

        total_balance += 0.0 if (company_details['holding'] is None) else company_details['holding']

        # Add to company list
        companies.append(company_details)

    # Obtain all user's cash
    query = db.execute("SELECT * FROM users WHERE id = :userID",
                       userID=session['user_id'])
    if not query:
        return apology("Account Error")

    cash = query[0]['cash']
    total_balance += cash

    return render_template("index.html", companies=companies, cash=cash, total_balance=total_balance)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Check symbol was entered
        if not request.form.get("symbol"):
            return apology("Missing Symbol!")

        # Check shares was entered
        if not request.form.get("shares"):
            return apology("Missing Shares!")

        # Lookup company by symbol
        quote = lookup(request.form.get("symbol").upper())

        # Check symbol was valid
        if quote is None:
            return apology("Unknown Symbol!")

        # Check shares is valid
        try:
            shares = int(request.form.get("shares"))

            if (shares == 0):
                return apology("Cannot buy 0 shares!")

            if not (shares > 0):
                return apology("Shares must be a positive whole number!")

        except:
            return apology("Shares must be a positive whole number!")

        # Calculate the price of the shares
        cost = shares * float(quote['price'])

        # Look up the user's cash
        cash = db.execute("SELECT cash FROM users WHERE id = :user_id",
                          user_id=session["user_id"])[0]['cash']
        try:
            cash = float(cash)
        except:
            apology("Cash not found")

        # Ensure uses has enough cash
        if cash < cost:
            return apology("You dont have enough!")

        # Obtain symbolID (or add to database if not already there)
        query = db.execute("SELECT symbolID FROM symbols WHERE symbol = :symbol",
                           symbol=quote['symbol'])
        if not query:
            symbolID = db.execute("INSERT INTO symbols (symbol) VALUES(:symbol)",
                                  symbol=quote['symbol'])
        else:
            symbolID = query[0]['symbolID']

        # Insert transaction into the database
        date_time = datetime.utcnow().strftime("%Y-%m-%d %H:%M:%S")
        transactionID = db.execute("INSERT INTO transactions (symbolID, userID, shares, price, \
                                   datetime, bought) VALUES(:symbolID, :userID, :shares, :price, \
                                   :datetime, :bought)",
                                   symbolID=symbolID, userID=session["user_id"],
                                   shares=shares, price=float(quote['price']),
                                   datetime=date_time, bought=1)
        if transactionID is None:
            return apology("Transaction not updated")

        # Update inventory
        query = db.execute("SELECT * FROM inventory WHERE symbolID = :symbolID AND userID = :userID",
                           symbolID=symbolID,  userID=session["user_id"])
        # If user doesn't already have shares in this company create new entry to table
        if not query:
            inventoryID = db.execute("INSERT INTO inventory (userID, symbolID, shares) \
                                     VALUES(:userID, :symbolID, :shares)",
                                     userID=session["user_id"], symbolID=symbolID,
                                     shares=shares)
        # Otherwise update initial entry
        else:
            current_shares = query[0]['shares']
            update = db.execute("UPDATE inventory SET shares = :updated_shares \
                                WHERE userID = :userID AND symbolID = :symbolID",
                                updated_shares=shares + current_shares,
                                userID=session["user_id"], symbolID=symbolID)
            if update is None:
                return apology("Shares not updated")

        # Update user's cash
        cash -= cost
        update = db.execute("UPDATE users SET cash = :cash WHERE id = :userID",
                            cash=cash, userID=session["user_id"])
        if update is None:
            return apology("Cash not updated")

        # Return user to index page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""

    # Obtain all user's transactions
    query = db.execute("SELECT * FROM transactions \
                       JOIN symbols ON transactions.symbolID = symbols.symbolID \
                       WHERE userID = :userID",
                       userID=session['user_id'])
    if query is None:
        return apology("History not found")

    # Obtain details for each company
    transactions = []
    for transaction in query:
        # Create dict to hold details
        transaction_details = {}

        # Lookup current name
        quote = lookup(transaction['symbol'])
        if quote is None:
            # If couldn't be found set name and price to empty
            quote = {'name': '', 'price': None, 'symbol': company['symbol']}

        transaction_details['name'] = quote['name']
        transaction_details['symbol'] = transaction['symbol']
        transaction_details['bought'] = "Bought" if transaction['bought'] else "Sold"
        transaction_details['shares'] = transaction['shares']
        transaction_details['price'] = transaction['price']
        transaction_details['datetime'] = transaction['datetime']

        # Add to company list
        transactions.append(transaction_details)

    return render_template("history.html", transactions=transactions)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username",
                          username=request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Check symbol was entered
        if not request.form.get("symbol"):
            return apology("Missing Symbol!")

        # Lookup symbol
        quote = lookup(request.form.get("symbol").upper())

        # Check symbol was valid
        if quote is None:
            return apology("Unknown Symbol!")

        # Render symbols information (quote contains 'name', 'price' and 'symbol')
        return render_template("quoted.html", **quote)

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Check username entered
        if not request.form.get("username"):
            return apology("Missing username!")

        # Check password and password confirmation entered
        if (not request.form.get("password")) or (not request.form.get("confirmation")):
            return apology("Missing password!")

        # Check password is between 6 and 20 chars long, and there is at least 1 digit
        if (len(request.form.get("password")) < 6) or (len(request.form.get("password")) > 20):
            return apology("Password must be between 6 and 20 characters long")
        digits = sum(c.isdigit() for c in request.form.get("password"))
        if digits < 1:
            return apology("Password must contain at least 1 digit")

        # Check password and password confirmation match
        if (request.form.get("password") != request.form.get("confirmation")):
            return apology("Passwords don't match!")

        # Add username to database in the table 'users' in the column 'username'
        result = db.execute("INSERT INTO users (username, hash) VALUES(:username, :hash)",
                            username=request.form.get("username"),
                            hash=generate_password_hash(request.form.get("password")))

        # Check this was successful
        if not result:
            return apology("Username taken!")

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username",
                          username=request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Check symbol was entered
        if not request.form.get("symbol"):
            return apology("Missing Symbol!")

        # Check shares was entered
        if not request.form.get("shares"):
            return apology("Missing Shares!")

        # Check shares is valid
        try:
            shares = int(request.form.get("shares"))

            if (shares == 0):
                return apology("Cannot sell 0 shares!")

            if not (shares > 0):
                return apology("Shares must be a positive whole number!")

        except:
            return apology("Shares must be a positive whole number!")

        # Lookup company by symbol
        quote = lookup(request.form.get("symbol").upper())

        # Check symbol was valid
        if quote is None:
            return apology("Unknown Symbol!")

       # Obtain symbolID
        query = db.execute("SELECT * FROM symbols WHERE symbol = :symbol",
                           symbol=quote['symbol'])
        if not query:
            return apology("Internal Symbol Error")
        symbolID = query[0]['symbolID']

        # Query for owned symbols
        query = db.execute("SELECT * FROM inventory \
                           JOIN symbols ON inventory.symbolID = symbols.symbolID \
                           WHERE userID = :userID AND symbols.symbolID = :symbolID",
                           userID=session['user_id'], symbolID=symbolID)
        if not query:
            return apology("Symbol shares not owned")

        # Check user owns enough shares in stock
        try:
            owned_shares = int(query[0]['shares'])

            if owned_shares < shares:
                return apology("Not enough shares owned")
        except:
            return apology("Internal Symbol Error")

        # Insert transaction into the database
        date_time = datetime.utcnow().strftime("%Y-%m-%d %H:%M:%S")
        transactionID = db.execute("INSERT INTO transactions (symbolID, userID, shares, price, \
                                   datetime, bought) VALUES(:symbolID, :userID, :shares, :price, \
                                   :datetime, :bought)",
                                   symbolID=symbolID, userID=session["user_id"],
                                   shares=shares, price=float(quote['price']),
                                   datetime=date_time, bought=0)
        if transactionID is None:
            return apology("Transaction not updated")

        # Update inventory
        updated_shares = owned_shares - shares
        if updated_shares == 0:
            delete = db.execute("DELETE FROM inventory WHERE userID = :userID AND symbolID = :symbolID",
                                userID=session["user_id"], symbolID=symbolID)
            if delete == 0:
                return apology("Couldn't remove shares")
        else:
            inventoryID = db.execute("UPDATE inventory SET shares = :updated_shares \
                                     WHERE userID = :userID AND symbolID = :symbolID",
                                     updated_shares=updated_shares,
                                     userID=session["user_id"], symbolID=symbolID)
            if inventoryID is None:
                return apology("Inventory not updated")

        # Calculate the price of the shares
        cost = shares * float(quote['price'])

        # Look up the user's cash
        cash = db.execute("SELECT cash FROM users WHERE id = :user_id",
                          user_id=session["user_id"])[0]['cash']
        cash = float(cash)

        # Update user's cash
        cash += cost
        update = db.execute("UPDATE users SET cash = :cash WHERE id = :userID",
                            cash=cash, userID=session["user_id"])
        if update is None:
            return apology("Cash not updated")

        # Return user to index page
        return redirect("/")

    else:

        # Query for owned symbols
        query = db.execute("SELECT * FROM inventory \
                           JOIN symbols ON inventory.symbolID = symbols.symbolID \
                           WHERE userID = :userID",
                           userID=session['user_id'])

        # Remove symbols
        symbols = [company['symbol'] for company in query]

        return render_template("sell.html", symbols=symbols)


def errorhandler(e):
    """Handle error"""
    return apology(e.name, e.code)


# listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)