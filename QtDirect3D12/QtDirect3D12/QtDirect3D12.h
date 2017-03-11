#pragma once

#include <QtWidgets/QWidget>
#include "ui_QtDirect3D12.h"

class QtDirect3D12 : public QWidget
{
	Q_OBJECT

public:
	QtDirect3D12(QWidget *parent = Q_NULLPTR);

protected:
	void resizeEvent(QResizeEvent *event) Q_DECL_FINAL;

private:
	Ui::QtDirect3D12Class ui;
};
