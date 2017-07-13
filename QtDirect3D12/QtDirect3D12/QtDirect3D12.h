#pragma once

#include <memory>
#include <QtWidgets/QWidget>
#include <Sein/Direct3D12/Direct3D12Device.h>
#include <Sein/Direct3D12/VertexBuffer.h>
#include <Sein/Direct3D12/IndexBuffer.h>
#include <Sein/Direct3D12/InstanceBuffer.h>
#include "ui_QtDirect3D12.h"

class QtDirect3D12 : public QWidget
{
	Q_OBJECT

public:
	QtDirect3D12(QWidget *parent = Q_NULLPTR);
	~QtDirect3D12();

protected:
	void resizeEvent(QResizeEvent *event) Q_DECL_FINAL;

private Q_SLOTS:
	void mainLoop();

private:
	Ui::QtDirect3D12Class ui;
	std::unique_ptr<QTimer> timer;
	std::unique_ptr<Sein::Direct3D12::Device> device;
	std::unique_ptr<Sein::Direct3D12::VertexBuffer> vertexBuffer;
	std::unique_ptr<Sein::Direct3D12::IndexBuffer> indexBuffer;
	std::unique_ptr<Sein::Direct3D12::InstanceBuffer> instanceBuffer;
};
