// Copyright © Michael Winkelmann <michael@winkelmann.site>
// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once

#include <QtWidgets>
#include "stepinterface.h"

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

class Dial : public QDial, public StepTrait<Dial> {
    Q_OBJECT
    Q_INTERFACES(StepInterface)
public:
    Dial(QWidget* parent = nullptr) : QDial(parent) {
        setMinimum(0);
        setMaximum(9);
    }

    QSize minimumSizeHint() const override {
        return {96, 96};
    }

    void setStep(int step) override {
        setValue(step);
        StepTrait<Dial>::setStep(step);
    }

};

class ProgressBar : public QProgressBar, public StepTrait<ProgressBar> {
    Q_OBJECT
    Q_INTERFACES(StepInterface)
public:
    ProgressBar(QWidget* parent = nullptr) : QProgressBar(parent) {
        setMinimum(0);
        setMaximum(9);
    }

    QSize minimumSizeHint() const override {
        return {120, 24};
    }

    void setStep(int step) override {
        setValue(step);
        StepTrait<ProgressBar>::setStep(step);
    }

};

class LineEdit : public QLineEdit, public StepTrait<LineEdit, 16> {
    Q_OBJECT
    Q_INTERFACES(StepInterface)
public:
    LineEdit(QWidget* parent = nullptr) : QLineEdit("Enter some text", parent) {
    }

    QSize minimumSizeHint() const override {
        return {120, 24};
    }

    void setStep(int step) override {
        setText(text.first(step));
        StepTrait<LineEdit, 16>::setStep(step);
    }

private:
    QString text = "Enter some text";
};

class Slider : public QSlider, public StepTrait<Slider> {
    Q_OBJECT
    Q_INTERFACES(StepInterface)
public:
    Slider(QWidget* parent = nullptr) : QSlider(parent) {
        setMinimum(0);
        setMaximum(9);
    }

    void setStep(int step) override {
        setValue(step);
        StepTrait<Slider>::setStep(step);
    }

    QSize minimumSizeHint() const override {
        return {22, 96};
    }
};


