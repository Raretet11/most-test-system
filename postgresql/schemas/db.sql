DROP SCHEMA IF EXISTS hello_schema CASCADE;

CREATE SCHEMA IF NOT EXISTS hello_schema;

CREATE TABLE IF NOT EXISTS hello_schema.users (
    name TEXT PRIMARY KEY,
    count INTEGER DEFAULT(1)
);

CREATE TABLE IF NOT EXISTS hello_schema.tasks (
    id SERIAL PRIMARY KEY,
    name TEXT,
    tests TEXT
);

CREATE TABLE IF NOT EXISTS hello_schema.solutions (
    id SERIAL PRIMARY KEY,
    task_id INTEGER REFERENCES hello_schema.tasks(id) ON DELETE CASCADE,
    code TEXT,
    verdict TEXT
);
