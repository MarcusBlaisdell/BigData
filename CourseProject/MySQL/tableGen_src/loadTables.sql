USE youtube
CREATE TABLE IF NOT EXISTS videos(
	video_id CHAR(11) NOT NULL PRIMARY KEY,
	uploader VARCHAR(20) NOT NULL,
	age MEDIUMINT UNSIGNED NOT NULL,
	category VARCHAR(25) NOT NULL,
	length MEDIUMINT UNSIGNED NOT NULL,
	views INT UNSIGNED NOT NULL,
	rate FLOAT NOT NULL,
	rating SMALLINT UNSIGNED NOT NULL
);
LOAD DATA LOCAL INFILE 'videos.txt' REPLACE INTO TABLE videos;

DROP TABLE related;
CREATE TABLE IF NOT EXISTS related(
	main_vid CHAR(11) NOT NULL,
	related_vid CHAR(11) NOT NULL
);
LOAD DATA LOCAL INFILE 'related.txt' REPLACE INTO TABLE related;