# CS 2337 Fall 2020 Project 4 - Ticket Reservation System

## Objectives
- Use a hashmap to access and store various pieces of data
- Create a comprehensive professional application

## Project Description
- Develop the backend for an online ticket reservation system
- Features: 
    + Allows attendees to select seats and display the current seating arrangement
    + Generate a report indicating how many seats were sold/unsold and how much money was earned
    + Login system for customers to: 
        - Add orders
        - Change orders
        - Cancel orders
        - See the receipts of their orders
    + An interface for adminstrator with the following features:
        - Print report

## Implementation Details
- 3 auditoriums will be available to reserve seats
- The seating arrangement for each auditorium will be stored in individual files
    + Auditorium 1 - <code>A1.txt</code>
    + Auditorium 2 - <code>A2.txt</code>
    + Auditorium 3 - <code>A3.txt</code>
- Reserved seats will be represented by a letter (A, S, or C) in the file with the following meaning: 
    + A - Adult
    + S - Senior
    + C - Children
- Reserved seats will be represented with a hashtag (#) on the screen
- Ticket price are as follow: 
    + Adult - $10
    + Children - $5
    + Senior - $7.50
- User data will be read from a file named <code>userdb.dat</code>
- The username of the administrator will be <code>admin</code>
- All user data should be filled in a hashmap before starting the user interface
- Upon exit, store each auditorium in a new file named <code>A#Final.txt</code>, where # will represent the auditorium number

