-- 创建blog数据库
CREATE
DATABASE blog


-- 创建 article表
create table article
(
    id         SERIAL PRIMARY KEY,
    title      TEXT      NOT NULL,
    content    TEXT      NOT NULL,
    summary    TEXT      NOT NULL,
    tags       INTEGER[]     NOT NULL DEFAULT '{}',
    created_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
    updated_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP
);


-- 创建 tag表
create table tag
(
    id    SERIAL PRIMARY KEY,
    name  TEXT NOT NULL UNIQUE,
    count INT  NOT NULL DEFAULT 0,
    path  TEXT NOT NULL DEFAULT '',
);


-- 创建 users表
create table "users"
(
    id          SERIAL PRIMARY KEY,
    username    TEXT      NOT NULL UNIQUE,
    nickname    TEXT      NOT NULL DEFAULT '',
    email       TEXT      NOT NULL DEFAULT '',
    headshot    TEXT      NOT NULL DEFAULT '',
    description TEXT      NOT NULL DEFAULT '',
    position    TEXT      NOT NULL DEFAULT '',
    area        TEXT      NOT NULL DEFAULT '',
    github      TEXT      NOT NULL DEFAULT '',
    telegram    TEXT      NOT NULL DEFAULT '',
    linkedin    TEXT      NOT NULL DEFAULT '',
    twitter     TEXT      NOT NULL DEFAULT '',
    tech_stack  TEXT[]      NOT NULL DEFAULT '{}',
    created_at  TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
    updated_at  TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP
);