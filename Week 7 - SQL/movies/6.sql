SELECT AVG(rating) AS average_ratings FROM ratings WHERE movie_id IN (SELECT id FROM movies WHERE year = 2012);
