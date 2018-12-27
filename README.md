# shabbat-pushbutton

Video demo: https://www.youtube.com/watch?v=IJEl0PeK9OM

## TODO:
* Test Shabbos mode (should work fine)
* Build mount for motors (expand this as necessary)
* Provide Fritzing Ref to github (optional)
* Solve power problem to run off 1 pack (optional)

## References:
https://learn.sparkfun.com/tutorials/deadon-rtc-breakout-hookup-guide?_ga=1.85307481.1994918977.1478696469

## Optmization Note:
Originally, used the RTC's alarm modes and only read time as necessary for display etc.
Now time is read on every loop, so it made sense to just use generic comparisons for alarm triggering
Advantage is that display could just be constant clock (though this isn't the case now) - and same code can be used for any rtc (without alarm functions)
Disadvantage is battery drain

Another place to potentially optimize would be to shut down display when inactive
