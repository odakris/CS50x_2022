-- Keep a log of any SQL queries you execute as you solve the mystery.

-- First let's look at the crime scene reports on the day and street where the theft happened
SELECT
   *
FROM
   crime_scene_reports
WHERE
   year = 2021
   AND month = 7
   AND day = 28
   AND street = "Humphrey Street";

--| 295 | 2021 | 7     | 28  | Humphrey Street | Theft of the CS50 duck took place at 10:15am at the Humphrey Street bakery.
--Interviews were conducted today with three witnesses who were present at the time – each of their interview transcripts mentions the bakery.

-- We know now that the theft took place at 10:15am and that there is three witnesses.

-- Then let's look at interviews that happened on the same day that mention the bakery
SELECT
   *
FROM
   interviews
WHERE
   year = 2021
   AND month = 7
   AND day = 28
   AND transcript LIKE "%bakery%";

-- HERE ARE INTERVIEWS
--| 161 | Ruth    |Sometime within ten minutes of the theft, I saw the thief get into a car in the bakery parking lot and drive away.
                --If you have security footage from the bakery parking lot, you might want to look for cars that left the parking lot in that time frame.
--| 162 | Eugene  |I don't know the thief's name, but it was someone I recognized.
                --Earlier this morning, before I arrived at Emma's bakery, I was walking by the ATM on Leggett Street and saw the thief there withdrawing some money.
--| 163 | Raymond |As the thief was leaving the bakery, they called someone who talked to them for less than a minute.
                --In the call, I heard the thief say that they were planning to take the earliest flight out of Fiftyville tomorrow.
                --The thief then asked the person on the other end of the phone to purchase the flight ticket.

-- 1 - LETS FIRST FOCUS ON RUTH'S IONTERVIEW
--| 161 | Ruth    |Sometime within ten minutes of the theft, I saw the thief get into a car in the bakery parking lot and drive away.
                --If you have security footage from the bakery parking lot, you might want to look for cars that left the parking lot in that time frame.

-- First we want to look at every cars that left the parking lot within 10min from 10:15am.
SELECT
   *
FROM
   bakery_security_logs
WHERE
   year = 2021
   AND month = 7
   AND day = 28
   AND hour = 10
   AND minute < 26
   AND minute > 15
   AND activity = "exit";

-- We know that we can connect BAKERY_SECURITY_LOGS and PEOPLE tables throught LICENCE_PLATE
-- This way wa can get names from every person that left the parking lot around 10:15am
SELECT
   name
FROM
   people
   JOIN
      bakery_security_logs
      ON people.license_plate = bakery_security_logs.license_plate
WHERE
   year = 2021
   AND month = 7
   AND day = 28
   AND hour = 10
   AND minute < 26
   AND minute > 15
   AND activity = "exit";

-- THIS GIVE US OUR FIRST SUSPECTS LIST
--+---------+
--|  name   |
--+---------+
--| Vanessa |
--| Bruce   |
--| Barry   |
--| Luca    |
--| Sofia   |
--| Iman    |
--| Diana   |
--| Kelsey  |
--+---------+

-- 2 - LET'S NOW LOOK AT THE SECOND INTERVIEW
--| 162 | Eugene  |I don't know the thief's name, but it was someone I recognized.
                --Earlier this morning, before I arrived at Emma's bakery, I was walking by the ATM on Leggett Street and saw the thief there withdrawing some money.

-- We now want to look at atm_transactions that happened on Leggett Street before the theft, so before 10:15am
SELECT
   *
FROM
   atm_transactions
WHERE
   year = 2021
   AND month = 7
   AND day = 28
   AND atm_location = "Leggett Street"
   AND transaction_type = "withdraw";

-- We know that we can link ATM_TRANSACTIONS and BANK_ACCOUNTS tables throught ACCOUNT_NUMBER
-- We also know that we can connect BANK_ACCOUNTS and PEOPLE tables throught PERSON_ID that refer to ID from PEOPLE table.
SELECT
   name
FROM
   people
   JOIN
      bank_accounts
      ON people.id = bank_accounts.person_id
   JOIN
      atm_transactions
      ON bank_accounts.account_number = atm_transactions.account_number
WHERE
   year = 2021
   AND month = 7
   AND day = 28
   AND atm_location = "Leggett Street"
   AND transaction_type = "withdraw";

-- THIS GIVE US A SECOND SUSPECTS LIST
--+---------+
--|  name   |
--+---------+
--| Bruce   |
--| Diana   |
--| Brooke  |
--| Kenny   |
--| Iman    |
--| Luca    |
--| Taylor  |
--| Benista |
--+---------+

-- 3 - THEN WE'LL GET INTO THE THIRD INTERVIEW FROM RAYMOND
--| 163 | Raymond |As the thief was leaving the bakery, they called someone who talked to them for less than a minute.
                --In the call, I heard the thief say that they were planning to take the earliest flight out of Fiftyville tomorrow.
                --The thief then asked the person on the other end of the phone to purchase the flight ticket.

 -- Here we have three informations:
                -- first  : the thief called his accomplice less than a minute while leaving the bakery
                -- second : He planned to take the earliest flight the day after the theft
                -- third  : it's the accomplice that purchased ticket flight (We'll focus on this third information once we identify the theft)

-- We will first look at phone calls on this day with a duration less than 60s
SELECT
   *
FROM
   phone_calls
WHERE
   year = 2021
   AND month = 7
   AND day = 28
   AND duration < 60;

-- first  : the thief called his accomplice less than a minute while leaving the bakery
-- Now we know we can connect PHONE_CALLS and PEOPLE tables throught PHONE_CALLS.CALLER = PEOPLE.PHONE_NUMBER
-- So now we can identify people names that called less than 60s on the day of the theft
SELECT
   name
FROM
   people
   JOIN
      phone_calls
      ON people.phone_number = phone_calls.caller
WHERE
   year = 2021
   AND month = 7
   AND day = 28
   AND duration < 60;

-- THIS GIVE US A THIRD SUSPECTS LIST
--+---------+
--|  name   |
--+---------+
--| Sofia   |
--| Kelsey  |
--| Bruce   |
--| Kelsey  |
--| Taylor  |
--| Diana   |
--| Carina  |
--| Kenny   |
--| Benista |
--+---------+

-- second : He took the earliest flight the day after the theft
-- So let's look at the earliest flight
SELECT
   id
FROM
   flights
WHERE
   year = 2021
   AND month = 7
   AND day = 29
   AND origin_airport_id =
   (
      SELECT
         id
      FROM
         airports
      WHERE
         city = "Fiftyville"
   )
ORDER by
   hour,
   minute LIMIT 1;

-- We know that we can connect FLIGHTS and PASSENGERS tables throught FLIGHTS.ID = PASSENGER.FLIGHT_ID
-- We also can connect PASSENGERS and PEOPLE tables throught PASSENGERS.PASSPORT_NUMBER = PEOPLE.PASSPORT_NUMBER
-- So now we can identify poeple names that took that flight
SELECT
   name
FROM
   people
   JOIN
      passengers
      ON people.passport_number = passengers.passport_number
WHERE
   flight_id IN
   (
      SELECT
         id
      FROM
         flights
      WHERE
         year = 2021
         AND month = 7
         AND day = 29
         AND origin_airport_id =
         (
            SELECT
               id
            FROM
               airports
            WHERE
               city = "Fiftyville"
         )
      ORDER by
         hour,
         minute LIMIT 1
   )
;

-- THIS GIVE US A FOURTH SUSPECTS LIST
--+--------+
--|  name  |
--+--------+
--| Doris  |
--| Sofia  |
--| Bruce  |
--| Edward |
--| Kelsey |
--| Taylor |
--| Kenny  |
--| Luca   |
--+--------+

-- FINAL : LET'S NOW INTERSECT EVERY SUSPECTS LISTS AND SEE IF WE GOT A NAME
SELECT
   name
FROM
   people
   JOIN
      bakery_security_logs
      ON people.license_plate = bakery_security_logs.license_plate
WHERE
   year = 2021
   AND month = 7
   AND day = 28
   AND hour = 10
   AND minute < 26
   AND minute > 15
   AND activity = "exit"
INTERSECT
SELECT
   name
FROM
   people
   JOIN
      bank_accounts
      ON people.id = bank_accounts.person_id
   JOIN
      atm_transactions
      ON bank_accounts.account_number = atm_transactions.account_number
WHERE
   year = 2021
   AND month = 7
   AND day = 28
   AND atm_location = "Leggett Street"
   AND transaction_type = "withdraw"
INTERSECT
SELECT
   name
FROM
   people
   JOIN
      phone_calls
      ON people.phone_number = phone_calls.caller
WHERE
   year = 2021
   AND month = 7
   AND day = 28
   AND duration < 60
INTERSECT
SELECT
   name
FROM
   people
   JOIN
      passengers
      ON people.passport_number = passengers.passport_number
WHERE
   flight_id IN
   (
      SELECT
         id
      FROM
         flights
      WHERE
         year = 2021
         AND month = 7
         AND day = 29
         AND origin_airport_id =
         (
            SELECT
               id
            FROM
               airports
            WHERE
               city = "Fiftyville"
         )
      ORDER by
         hour,
         minute LIMIT 1
   )
;

-- WE FOUND HIM !! THE THEFT IS NO ONE THAN BRUCE !!
--+--------+
--|  name  |
--+--------+
--| Bruce  |
--+--------+


-- LET'S NOW DISCOVER WHERE HE ESCAPED
-- Now that we know the theft's name we can easily identify where he left
SELECT
   city
FROM
   airports
WHERE
   id =
   (
      SELECT
         destination_airport_id
      FROM
         flights
      WHERE
         id =
         (
            SELECT
               flight_id
            FROM
               passengers
            WHERE
               passport_number =
               (
                  SELECT
                     passport_number
                  FROM
                     people
                  WHERE
                     name = "Bruce"
               )
         )
   )
;

-- BRUCE LEFT TO NYC
--+---------------+
--|     city      |
--+---------------+
--| New York City |
--+---------------+

-- FINALLY LET'S FOUND OUT WHO IS HIS ACCOMPLICE
SELECT
   name
FROM
   people
WHERE
   phone_number =
   (
      SELECT
         receiver
      FROM
         phone_calls
      WHERE
         caller =
         (
            SELECT
               phone_number
            FROM
               people
            WHERE
               name = "Bruce"
         )
         AND year = 2021
         AND month = 7
         AND day = 28
         AND duration < 60
   )
;

-- HIS ACCOMPLICE IS ROBIN
--+-------+
--| name  |
--+-------+
--| Robin |
--+-------+

-- RESULT --
-- The THIEF is: BRUCE
-- The city the thief ESCAPED TO: NEW YORK CITY
-- The ACCOMPLICE is: ROBIN
