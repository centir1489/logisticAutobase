DO $$
BEGIN
    IF NOT EXISTS (SELECT 1 FROM pg_roles WHERE rolname = 'autobase_user') THEN
        CREATE USER autobase_user WITH PASSWORD 'ABc324gR!0';
        ALTER USER autobase_user WITH SUPERUSER;
        GRANT ALL PRIVILEGES ON DATABASE logisticAutobase TO autobase_user;
    END IF;
END $$;

CREATE TABLE IF NOT EXISTS users (
    user_id SERIAL PRIMARY KEY,
    username VARCHAR(50) NOT NULL UNIQUE,
    passwd VARCHAR(255) NOT NULL
);

CREATE TABLE IF NOT EXISTS user_tables (
    table_id SERIAL PRIMARY KEY,
    user_id INT NOT NULL,
    table_name VARCHAR(50) NOT NULL UNIQUE,
    departure_location VARCHAR(100) NOT NULL,
    destination_location VARCHAR(100) NOT NULL,
    FOREIGN KEY (user_id) REFERENCES users(user_id) ON DELETE CASCADE
);

CREATE TABLE IF NOT EXISTS parcels (
    parcel_id SERIAL PRIMARY KEY,
    number_parcel VARCHAR(50) NOT NULL,
    sender VARCHAR(100) NOT NULL,
    recipient VARCHAR(100) NOT NULL,
    date_dispatch DATE NOT NULL,
    delivery_date DATE NOT NULL,
    status_parcel VARCHAR(50) NOT NULL,
    weight DECIMAL(10, 2) NOT NULL,
    table_id INT NOT NULL,
    FOREIGN KEY (table_id) REFERENCES user_tables(table_id) ON DELETE CASCADE
);

CREATE INDEX IF NOT EXISTS idx_user_tables_user_id ON user_tables(user_id);
CREATE INDEX IF NOT EXISTS idx_parcels_table_id ON parcels(table_id);
