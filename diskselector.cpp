#include "diskselector.h"
#include "ui_diskselector.h"
#include <QFocusEvent>
#include <QDebug>

#include <windows.h>
#include <string>
#include "Shtypes.h"
#include "Shlobj.h"

bool openShellContextMenuForObject(const std::wstring &path, int xPos, int yPos, void * parentWindow)
{
    Q_ASSERT(parentWindow);
    ITEMIDLIST * id = 0;
    std::wstring windowsPath = path;
    std::replace(windowsPath.begin(), windowsPath.end(), '/', '\\');
    HRESULT result = SHParseDisplayName(windowsPath.c_str(), 0, &id, 0, 0);
    if (!SUCCEEDED(result) || !id)
        return false;
    //CItemIdListReleaser idReleaser (id);

    IShellFolder * ifolder = 0;

    LPCITEMIDLIST idChild = 0;
    result = SHBindToParent(id, IID_IShellFolder, (void**)&ifolder, &idChild);
    if (!SUCCEEDED(result) || !ifolder)
        return false;
    //CComInterfaceReleaser ifolderReleaser (ifolder);

    IContextMenu * imenu = 0;
    result = ifolder->GetUIObjectOf((HWND)parentWindow, 1, &idChild, IID_IContextMenu, 0, (void**)&imenu);
    if (!SUCCEEDED(result) || !ifolder)
        return false;
    //CComInterfaceReleaser menuReleaser(imenu);

    HMENU hMenu = CreatePopupMenu();
    if (!hMenu)
        return false;
    if (SUCCEEDED(imenu->QueryContextMenu(hMenu, 0, 1, 0x7FFF, CMF_NORMAL)))
    {
        int iCmd = TrackPopupMenuEx(hMenu, TPM_RETURNCMD, xPos, yPos, (HWND)parentWindow, NULL);
        if (iCmd > 0)
        {
            CMINVOKECOMMANDINFOEX info = { 0 };
            info.cbSize = sizeof(info);
            info.fMask = CMIC_MASK_UNICODE;
            info.hwnd = (HWND)parentWindow;
            info.lpVerb = MAKEINTRESOURCEA(iCmd - 1);
            info.lpVerbW = MAKEINTRESOURCEW(iCmd - 1);
            info.nShow = SW_SHOWNORMAL;
            imenu->InvokeCommand((LPCMINVOKECOMMANDINFO)&info);
        }
    }
    DestroyMenu(hMenu);
    return true;
}


DiskSelector::DiskSelector(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DiskSelector)
{
    ui->setupUi(this);
    model = new QFileSystemModel(this);
    model->setRootPath("");
    model->setFilter(QDir::Drives);
    ui->listView->setModel(model);
    ui->listView->installEventFilter(this);

    ui->listView->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->listView, &QListView::customContextMenuRequested,
            this, &DiskSelector::onCustomContextMenu);
}

DiskSelector::~DiskSelector()
{
    delete ui;
}

void DiskSelector::showEvent(QShowEvent *event)
{
    int w = 50 + ui->listView->sizeHintForColumn(0) ;
    int h = 10 + ui->listView->sizeHintForRow(0) * ui->listView->model()->rowCount();

    qDebug() << "size:" <<ui->listView->model()->rowCount();

    resize(w,h);
    ui->listView->setFocus();
    ui->listView->setCurrentIndex(ui->listView->currentIndex());
}

void DiskSelector::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Return ||
            event->key() == Qt::Key_Enter) {
        QString path = model->filePath(ui->listView->currentIndex());
        emit driveSelected(QDir(path));
        hide();
        event->accept();
        return;
    }

    if (event->key() == Qt::Key_Escape) {
        hide();
        event->accept();
        return;
    }
    event->ignore();
}

bool DiskSelector::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == ui->listView && event->type() == QEvent::FocusOut){
        hide();
        event->accept();
        return true;
    }
    return QWidget::eventFilter(obj, event);
}

void DiskSelector::on_listView_clicked(const QModelIndex &index)
{
    QString path = model->filePath(index);
    emit driveSelected(QDir(path));
    hide();
}


void DiskSelector::onCustomContextMenu(const QPoint &point)
{
    auto index = ui->listView->indexAt(point);
    if (index.isValid()) {
        QString path =model->fileName(index);
        openShellContextMenuForObject(path.toStdWString(), point.x(), point.y(), (HWND)winId());
    }
}
