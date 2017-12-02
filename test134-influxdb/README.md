Launch the server:

    $ cd server
    $ docker-compose up

Add users and DB:

    $ cd server
    $ docker-compose exec influxdb influx
    > CREATE USER root WITH PASSWORD 'root' WITH ALL PRIVILEGES
    > CREATE USER alice WITH PASSWORD 'alice'
    > CREATE DATABASE "testdb"
    > GRANT READ ON "testdb" TO "alice"
    > GRANT WRITE ON "testdb" TO "alice"
