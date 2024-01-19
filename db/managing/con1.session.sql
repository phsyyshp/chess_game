SELECT * 
FROM games
;

SELECT white,  Count(*)
FROM games
Group BY white
;