NAME          Lemonade
ROWS
 N  PROFIT
 L  TIME
 L  LEMONS
 L  SUGAR
 L  WATER
 L  VODKA
COLUMNS
    REGULAR   PROFIT    1.             TIME      0.25
    REGULAR   LEMONS    1.             SUGAR     2.
    REGULAR   WATER     2.             VODKA     0.
    SPECIAL   PROFIT    2.             TIME      0.5
    SPECIAL   LEMONS    1.             SUGAR     1.25
    SPECIAL   WATER     0.6            VODKA     0.5
RHS
    RHS       TIME      60.            LEMONS    200.
    RHS       SUGAR     250.           WATER     240.
    RHS       VODKA     50.
BOUNDS
 LO BND       REGULAR   0.
 LO BND       SPECIAL   0.
ENDATA
