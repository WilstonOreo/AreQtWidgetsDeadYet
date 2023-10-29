#pragma once

#include <QtWidgets>


#include "model.h"

class TableView : public QTableView {
    Q_OBJECT
public:
    TableView(QWidget* parent = nullptr) :
        QTableView(parent) {
        setModel(new Model(1000, 10, this));
        setSelectionModel( new QItemSelectionModel(model()));
        horizontalHeader()->setSectionsMovable(true);
        verticalHeader()->setSectionsMovable(true);
        viewport()->setAttribute(Qt::WA_StaticContents);
    }

    QSize minimumSizeHint() const override {
        return {200, 200};
    }
};

class TreeView : public QTreeView {
    Q_OBJECT
public:
    TreeView(QWidget* parent = nullptr) :
        QTreeView(parent) {
        setModel(new Model(1000, 10, this));
        setUniformRowHeights(true);
        setSelectionModel( new QItemSelectionModel(model()));
        header()->setStretchLastSection(false);
        viewport()->setAttribute(Qt::WA_StaticContents);
    }

    QSize minimumSizeHint() const override {
        return {200, 200};
    }
};

class ListView : public QListView {
    Q_OBJECT
public:
    ListView(QWidget* parent = nullptr) :
        QListView(parent) {
        setModel(new Model(1000, 10, this));
        setSelectionModel( new QItemSelectionModel(model()));
        setViewMode(QListView::IconMode);
        setSelectionMode(QAbstractItemView::ExtendedSelection);
        setAlternatingRowColors(false);
        viewport()->setAttribute(Qt::WA_StaticContents);
        setAttribute(Qt::WA_MacShowFocusRect, false);
    }

    QSize minimumSizeHint() const override {
        return {200, 200};
    }
};

class Dial : public QDial {
    Q_OBJECT
public:
    Dial(QWidget* parent = nullptr) : QDial(parent) {
    }

    QSize minimumSizeHint() const override {
        return {96, 96};
    }
};

class LineEdit : public QLineEdit {
    Q_OBJECT
public:
    LineEdit(QWidget* parent = nullptr) : QLineEdit("Enter some text", parent) {
    }

    QSize minimumSizeHint() const override {
        return {120, 24};
    }
};

