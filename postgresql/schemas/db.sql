DROP SCHEMA IF EXISTS most_db CASCADE;

CREATE SCHEMA IF NOT EXISTS most_db;

CREATE TABLE IF NOT EXISTS most_db.tasks (
    id SERIAL PRIMARY KEY,
    name TEXT,
    tests TEXT,
    legend TEXT,
    time_limit INTEGER,
    memory_limit INTEGER
);

CREATE TABLE IF NOT EXISTS most_db.solutions (
    id SERIAL PRIMARY KEY,
    task_id INTEGER REFERENCES most_db.tasks(id) ON DELETE CASCADE,
    language TEXT,
    code TEXT,
    time_limit INTEGER,
    verdict TEXT,
    last_test INTEGER
);
