-- SELECT * 
-- FROM games
-- -- sort date 
-- ORDER BY date DESC
-- LIMIT 10
-- ;

-- SELECT *
-- FROM games
-- -- sif site have TUR
-- WHERE site LIKE '%TUR%'
-- LIMIT 100
-- ; 

-- SELECT ECO,
--        SUM(result = '1-0') *100 / COUNT(*) AS white_wins_percentage,
--        SUM(result = '0-1') * 100/ COUNT(*) AS black_wins_percentage,
--        SUM(result = '1/2-1/2') * 100/ COUNT(*) AS draws_percentage
-- FROM games
-- GROUP BY ECO
-- ORDER BY COUNT(*) DESC
-- LIMIT 100;


SELECT ECO,
-- SUM(WhiteElo>0 AND WhiteElo<1000) AS '0-1000',
-- SUM(WhiteElo>=1000 AND WhiteElo<1200) AS '1000-1200',
-- SUM(WhiteElo>=1200 AND WhiteElo<1400) AS '1200-1400',
-- SUM(WhiteElo>=1400 AND WhiteElo<1600) AS '1400-1600',
-- SUM(WhiteElo>=1600 AND WhiteElo<1800) AS '1600-1800',
-- SUM(WhiteElo>=1800 AND WhiteElo<2000) AS '1800-2000',
-- SUM(WhiteElo>=2000 AND WhiteElo<2200) AS '2000-2200',
SUM(WhiteElo>=2200 AND WhiteElo<2300)*100/COUNT(*) AS '2200-2400',
SUM(WhiteElo>=2300 AND WhiteElo<2400)* 100/
COUNT(*) AS '2300-2400',
SUM(WhiteElo>=2400 AND WhiteElo<2500)* 100/
COUNT(*) AS '2400-2500',
SUM(WhiteElo>=2500 AND WhiteElo<2600)* 100/
COUNT(*) AS '2500-2600',
SUM(WhiteElo>=2600 AND WhiteElo<2700)* 100/
COUNT(*) AS '2600-2700',
SUM(WhiteElo>=2700 AND WhiteElo<2800)* 100/
COUNT(*) AS '2700-2800',
SUM(WhiteElo>=2800 AND WhiteElo<2900)* 100/
COUNT(*) AS '2800-2900',
SUM(WhiteElo>=2900 AND WhiteElo<3000)* 100/
COUNT(*) AS '2900-3000',
SUM(WhiteElo>=3200 AND WhiteElo<3400) AS '3200-3400',
SUM(WhiteElo>=3400 AND WhiteElo<3600) AS '3400-3600',
SUM(WhiteElo>=3600 AND WhiteElo<3800) AS '3600-3800',
SUM(WhiteElo>=3800 AND WhiteElo<4000) AS '3800-4000'
FROM games
GROUP BY ECO
ORDER BY COUNT(*) DESC
LIMIT 100;



