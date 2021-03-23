import turtle
import time
import random
import winsound

# setting for the game screen
m2 = turtle.Screen()
m2.title("Snake Game for Lab 2")
m2.bgcolor("green")
m2.setup(width=700, height=700)   # screen size
m2.tracer(0)      # turns off the screen updates

# creating a Snake's head
head = turtle.Turtle()
head.speed(0)     # can be changed
head.shape("square")
head.color("black")
head.penup()
head.goto(0,0)    # Snake's starting point
head.direction = "stop"

# creating an Apple
food = turtle.Turtle()
food.speed(0)     # can be changed
food.shape("circle")
food.color("red")
food.penup()
food.goto(0,200)  # location of apple

# Moving functions
def go_up():
    head.direction = "up"
def go_down():
    head.direction = "down"
def go_right():
    head.direction = "right"
def go_left():
    head.direction = "left"
def move():
    if head.direction == "up":
        head.sety(head.ycor() + 20)   # y = head.ycor()
    if head.direction == "down":
        head.sety(head.ycor() - 20)
    if head.direction == "left":
        head.setx(head.xcor() - 20)   # x = head.xcor()
    if head.direction == "right":
        head.setx(head.xcor() + 20)

# Delay setting
delay = 0.1     # 0.1 seconds

# beep sound setting (need to be removed after setting a buzzer)
frequency = 2500
duration = 1000

# Keyboard setting
m2.listen()
m2.onkeypress(go_up, "w")
m2.onkeypress(go_down, "a")
m2.onkeypress(go_right, "s")
m2.onkeypress(go_left, "d")

# Main: Game Loop
while True:
    m2.update() 

    # setting when Snake passed through an Apple (beeping the buzzer)
    if head.distance(food) < 20:
        # beeping the buzzer
        # this part need to be replaced with using buzzer
        winsound.Beep(frequency, duration)
        # I think we should also add removing an apple 
        # after Snake passes apple.. but not sure

    # calling a Moving function
    move() 
    # moving + delaying for a few seconds
    time.sleep(delay)

m2.mainloop()
