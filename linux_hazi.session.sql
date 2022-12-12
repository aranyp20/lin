-- @block

drop table Tasks;

drop table Finished;


drop table Users;

drop table Courses;


CREATE TABLE Courses(
    course_id INT AUTO_INCREMENT,
    code VARCHAR(10),

    CONSTRAINT course_id_pk_c PRIMARY KEY (course_id),
    CONSTRAINT code_unique_c UNIQUE (code)
);
ALTER TABLE Courses AUTO_INCREMENT=0;


CREATE TABLE Users(
    user_id INT AUTO_INCREMENT,
    username VARCHAR(20),
    reward INT DEFAULT 0,
    reputation DECIMAL DEFAULT 0.00,

    CONSTRAINT user_id_pk_c PRIMARY KEY (user_id),
    CONSTRAINT username_unique_c UNIQUE (username)
);
ALTER TABLE Users AUTO_INCREMENT=0;

CREATE TABLE Finished(
    user_id INT,
    course_id INT,

    CONSTRAINT user_id_fk_c FOREIGN KEY (user_id) REFERENCES Users(user_id),
    CONSTRAINT course_id_fk_c FOREIGN KEY (course_id) REFERENCES Courses(course_id)
);


CREATE TABLE Tasks(
    task_id INT AUTO_INCREMENT,
    code VARCHAR(10),
    reward INT,
    due DATE DEFAULT '2030-01-01',

    master_id INT,
    slave_id INT DEFAULT 0,
    required_finished_id INT DEFAULT 0,

    CONSTRAINT task_id_pk_c PRIMARY KEY (task_id),
    CONSTRAINT master_id_fk_c FOREIGN KEY (master_id) REFERENCES Users(user_id)
);
ALTER TABLE Tasks AUTO_INCREMENT=0;




INSERT INTO Courses(code) VALUES('BMEVII001');
INSERT INTO Courses(code) VALUES('BMEVII002');
INSERT INTO Courses(code) VALUES('BMEVII003');
INSERT INTO Courses(code) VALUES('BMEVII004');
INSERT INTO Courses(code) VALUES('BMEVII005');

INSERT INTO Users(username) VALUES('Teszt_Elek');
INSERT INTO Users(username) VALUES('Minta_Janos');
INSERT INTO Users(username) VALUES('Bob');
INSERT INTO Users(username) VALUES('Eros_Pista');
INSERT INTO Users(username) VALUES('Mary');

INSERT INTO Finished(user_id,course_id) VALUES(1,1);
INSERT INTO Finished(user_id,course_id) VALUES(2,1);
INSERT INTO Finished(user_id,course_id) VALUES(1,2);
INSERT INTO Finished(user_id,course_id) VALUES(4,1);
INSERT INTO Finished(user_id,course_id) VALUES(4,2);


