

-- @block
drop table Tasks;

drop table Finished;

drop table Courses;

drop table Users;


-- @block

SELECT * FROM Finished;

-- @block


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
    slave_id INT,
    required_task_id INT,

    CONSTRAINT task_id_pk_c PRIMARY KEY (task_id),
    CONSTRAINT code_unique_c UNIQUE (code),
    CONSTRAINT master_id_fk_c FOREIGN KEY (master_id) REFERENCES Users(user_id),
    CONSTRAINT slave_id_fk_c FOREIGN KEY (slave_id) REFERENCES Users(user_id),
    CONSTRAINT required_task_id_fk_ck FOREIGN KEY (required_task_id) REFERENCES Courses(course_id),
    CONSTRAINT master_not_slave_ck_c CHECK(master_id != slave_id)
);


-- @block



INSERT INTO Courses(code) VALUES('BMEVII001');
INSERT INTO Courses(code) VALUES('BMEVII002');
INSERT INTO Courses(code) VALUES('BMEVII003');
INSERT INTO Courses(code) VALUES('BMEVII004');
INSERT INTO Courses(code) VALUES('BMEVII005');

INSERT INTO Users(username) VALUES('Teszt Elek');
INSERT INTO Users(username) VALUES('Minta Janos');
INSERT INTO Users(username) VALUES('Bob');
INSERT INTO Users(username) VALUES('Eros Pista');
INSERT INTO Users(username) VALUES('Mary');

INSERT INTO Finished(user_id,course_id) VALUES(1,1);
INSERT INTO Finished(user_id,course_id) VALUES(2,1);
INSERT INTO Finished(user_id,course_id) VALUES(1,2);
INSERT INTO Finished(user_id,course_id) VALUES(4,1);
INSERT INTO Finished(user_id,course_id) VALUES(4,2);








