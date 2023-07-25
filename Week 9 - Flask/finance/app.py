
import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.security import check_password_hash, generate_password_hash
from datetime import date, datetime


from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""
    # Getting all transaction of current user
    user_rows = db.execute(
        "SELECT * FROM portfolio WHERE id = ?", session["user_id"])

    # Getting cash of current user
    user_cash = db.execute(
        "SELECT cash FROM users WHERE id = ?", session["user_id"])[0]["cash"]

    # Default user cash
    total = user_cash

    for i in range(len(user_rows)):
        # Getting current price of shares owned by user
        current_price = lookup(user_rows[i]["share_symbol"])

        # UPDATE current share prices into database
        db.execute("UPDATE portfolio SET share_price = ? WHERE id = ? AND share_symbol = ?",
                   current_price["price"], session["user_id"], current_price["symbol"])

        # Calc of total cash of user
        total += user_rows[i]["share_price"]*user_rows[i]["number_share"]

    return render_template("index.html", user_rows=user_rows, cash=user_cash, usd=usd, total=total)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    if request.method == "POST":

        # getting remaining cash from current user ==> cash[0]["cash"]
        user_row = db.execute(
            "SELECT * FROM users WHERE id = ?", session["user_id"])

        # getting shares symbol asking by user
        symbol = request.form.get("symbol").upper()

        # getting number of shares
        shares = request.form.get("shares")

        # getting price's share of current symbol
        price = lookup(symbol)

        # getting current DATE
        current_date = date.today()

        # getting current TIME
        now = datetime.now()
        current_time = now.strftime("%H:%M:%S")

        # Checking for valide shares
        try:
            # Check if share user input is non-alphabetic
            shares = int(shares)

            # Check if it's a positive integer
            if shares < 1:
                return apology("Enter positive share", 400)
        except:
            return apology("Invalid Share", 400)

        # checking if symbol is valid
        if not price:
            return apology("Invalid Symbol", 400)

        # number of share * price of share
        sub_total = float(shares)*price["price"]

        # remaining cash after purchased
        cash = user_row[0]["cash"] - sub_total
        total = cash + sub_total

        # checking if user can afford it
        if cash < 0:
            return apology("Can't afford this !", 400)

        # Loading user portfolio
        portfolio = db.execute(
            "SELECT share_symbol FROM portfolio WHERE id = ?", session["user_id"])

        # getting number of shares owned by user
        user_shares = db.execute(
            "SELECT number_share FROM portfolio WHERE id = ? AND share_symbol = ?", session["user_id"], symbol)

        # Checking if user already get actual share
        for row in portfolio:
            if row["share_symbol"] == symbol:

                # update the number of shares owned by user
                db.execute("UPDATE portfolio SET number_share = ? WHERE id = ? AND share_symbol = ?",
                           (shares+user_shares[0]["number_share"]), session["user_id"], symbol)

                # INSERT purchased into HISTORY
                db.execute("INSERT INTO history (id, date, time, share_symbol, number_share, share_price, type, share_name) VALUES (?, ?, ?, ?, ?, ?, ?, ?)",
                           session["user_id"], current_date, current_time, symbol, shares, price["price"], "BUY", price["name"])

                # updating available cash of current user
                db.execute(
                    "UPDATE users SET cash = ? WHERE users.id = ?", cash, session["user_id"])

                return redirect("/")

        # INSERT purchased into HISTORY
        db.execute("INSERT INTO portfolio (id, share_symbol, number_share, share_price, share_name) VALUES (?, ?, ?, ?, ?)",
                   session["user_id"], symbol, shares, price["price"], price["name"])

        # If user can afford it, then record every details of the purchased
        db.execute("INSERT INTO history (id, date, time, share_symbol, number_share, share_price, type, share_name) VALUES (?, ?, ?, ?, ?, ?, ?, ?)",
                   session["user_id"], current_date, current_time, symbol, shares, price["price"], "BUY", price["name"])

        # updating available cash of current user
        db.execute("UPDATE users SET cash = ? WHERE users.id = ?",
                   cash, session["user_id"])

        return redirect("/")

    return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    # Selecting every transaction saved into HISTORY database
    user_history = db.execute(
        "SELECT * FROM history WHERE id = ?", session["user_id"])

    return render_template("history.html", user_history=user_history, usd=usd)


@app.route("/change_password", methods=["GET", "POST"])
@login_required
def change_password():
    """Allow user to change password"""

    if request.method == "POST":

        # Select user row
        user_row = db.execute(
            "SELECT * FROM users WHERE id = ?", session["user_id"])

        # Ensure old password was submitted
        if not request.form.get("old_password") or not check_password_hash(user_row[0]["hash"], request.form.get("old_password")):
            return apology("invalid old password", 403)

        # Ensure new password was submitted
        if not request.form.get("new_password"):
            return apology("must provide new password", 400)

        # Ensure confirmation was submitted
        elif not request.form.get("confirmation"):
            return apology("must confirme password", 400)

        # Ensure new password match
        if request.form.get("new_password") != request.form.get("confirmation"):
            return apology("new passwords don't match", 400)
        else:
            # HASH new password
            hash = generate_password_hash(request.form.get("confirmation"))
            # UPDATE new password in database
            db.execute("UPDATE users SET hash = ? WHERE id = ?",
                       hash, session["user_id"])

            return redirect("/")

    return render_template("change_password.html")


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
        rows = db.execute("SELECT * FROM users WHERE username = ?",
                          request.form.get("username"))

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

    # If POST
    if request.method == "POST":

        # getting symbol prompt by user
        symbol = request.form.get("symbol")

        # call lookup function that return name/symbol/price
        quoted = lookup(symbol)

        # if user prompt invalid stock symbol
        if not quoted:
            return apology("invalid Symbol", 400)

        # return information of stock prompt by user in front html
        return render_template("quoted.html", quoted=quoted, usd=usd)

    # return html that prompt user for stock symbol
    return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 400)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 400)

        # Ensure password match
        if request.form.get("password") != request.form.get("confirmation"):
            return apology("passwords don't match", 400)
        else:

            # store username and password has into variables
            username = request.form.get("username")
            hash = generate_password_hash(request.form.get("confirmation"))

        # if everything is good then store user info into database
        try:
            db.execute(
                "INSERT INTO users (username, hash) VALUES (?, ?)", username, hash)
            return redirect("/")
        except:
            # return apology if username is already taken
            return apology("username already taken", 400)

    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""

    # getting user cash
    user_cash = db.execute(
        "SELECT cash FROM users WHERE id  = ?", session["user_id"])

    # Select every stock owned by user
    user_rows = db.execute(
        "SELECT * FROM portfolio WHERE id = ?", session["user_id"])

    symbols = []

    for row in user_rows:
        if row["share_symbol"] not in symbols:
            # add every share symbol to symbols variable
            symbols.append(row)

    if request.method == "POST":
        #  getting symbol that user want to sell
        sell_symbol = request.form.get("symbol")

        # getting shares that user want to sell
        sell_shares = request.form.get("shares")

        # getting number of actual share owned by user
        user_share = db.execute(
            "SELECT number_share FROM portfolio WHERE id = ? AND share_symbol = ?", session["user_id"], sell_symbol)

        # Checking for valide shares
        try:
            # Check if share user input is non-alphabetic
            sell_shares = int(sell_shares)

            # Check if it's a positive integer
            if sell_shares < 1:
                return apology("Enter positive share", 400)
            # Check if shares that user wants to sell do not exceed number of actual shares he got
            if sell_shares > user_share[0]["number_share"]:
                return apology("You don't owned that many shares", 400)
        except:
            return apology("SUCK", 400)

        # Calculation of delta between shares owned by user and shares user want to sell
        delta_share = user_share[0]["number_share"] - sell_shares

        # getting price's share of current symbol
        price = lookup(sell_symbol)

        # getting current DATE
        current_date = date.today()

        # getting current TIME
        now = datetime.now()
        current_time = now.strftime("%H:%M:%S")

        # If user want to sell all shares he got
        if delta_share == 0:
            # Delete shares from portfolio
            db.execute("DELETE FROM portfolio WHERE id = ? AND share_symbol = ?",
                       session["user_id"], sell_symbol)

        # if user does not sell every shares
        else:
            # UPDATE number of shares remaining
            db.execute("UPDATE portfolio SET number_share = ? WHERE id = ? AND share_symbol = ?",
                       delta_share, session["user_id"], sell_symbol)

        # Sold is how much money is return to user
        sold = sell_shares * price["price"]

        # UPDATE user cash
        db.execute("UPDATE users SET cash = ? WHERE id = ?",
                   sold+user_cash[0]["cash"], session["user_id"])

        #  INSERT transaction into history database
        db.execute("INSERT INTO history (id, date, time, share_symbol, number_share, share_price, type, share_name) VALUES (?, ?, ?, ?, ?, ?, ?, ?)",
                   session["user_id"], current_date, current_time, sell_symbol, sell_shares, price["price"], "SELL", price["name"])

        return redirect("/")

    return render_template("sell.html", symbols=symbols)
