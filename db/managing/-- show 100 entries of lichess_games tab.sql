-- show 100 entries of lichess_games table
-- SELECT * FROM lichess_games 
-- SELECT DISTINCT White FROM lichess_games
-- UNION
-- SELECT DISTINCT Black FROM lichess_games;
-- SELECT DISTINCT opening FROM lichess_games
-- ORDER BY opening ASC;


SELECT opening, count(id) FROM lichess_games
GROUP BY opening
ORDER BY opening ASC;






