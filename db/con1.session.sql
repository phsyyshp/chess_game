SELECT id, white, black, date, WhiteELo
FROM games
WHERE WhiteELo > 10
ORDER BY
WhiteELo DESC
; 
--
SELECT *
FROM games
LIMIT 100 
;
--
SELECT id, white, black, date, WhiteELo
FROM games
WHERE WhiteELo > 10 AND WhiteELo IS NOT NULL
ORDER BY WhiteELo DESC;
---

SELECT COUNT(*)
FROM games
;