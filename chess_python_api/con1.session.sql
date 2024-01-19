INSERT INTO games (
  id, 
  event, 
  site, 
  date, 
  round, 
  white_player_id, 
  black_player_id, 
  result, 
  utc_date, 
  utc_time, 
  eco, 
  opening, 
  time_control, 
  termination, 
  moves
)
SELECT 
  id, 
  Event, 
  Site, 
  Date, 
  Round,
  (SELECT players.id FROM players WHERE players.name = White),
  (SELECT players.id FROM players WHERE players.name = Black),
  Result, 
  UTCDate, 
  UTCTime, 
  ECO, 
  Opening, 
  TimeControl, 
  Termination, 
  moves
FROM lichess_games;
