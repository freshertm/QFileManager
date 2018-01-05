#include "dirviewer.h"
#include "ui_dirviewer.h"
#include <QKeyEvent>
#include "customevent.h"


#include <windows.h>
#include <string>
#include "Shtypes.h"
#include "Shlobj.h"

DirViewer::DirViewer(QDir dir, QWidget *parent) :
    AbstractPanel(dir, parent),
    ui(new Ui::DirViewer),
    dir(dir)
{
    ui->setupUi(this);

    model = new QFileSystemModel(this);
    model->setRootPath(dir.absolutePath());
    model->setFilter(QDir::NoDot | QDir::AllEntries);
    ui->listView->setModel(model);
    changeDir(dir);

    selector = new DiskSelector(this);
    selector->setVisible(false);

    selector->installEventFilter(this);

    connect(selector, &DiskSelector::driveSelected, this, &DirViewer::changeDir);
    connect(model, &QFileSystemModel::directoryLoaded, this, &DirViewer::directoryLoaded);
    connect(ui->listView, &QListView::customContextMenuRequested,
            this, &DirViewer::onCustomContextMenu);
}

DirViewer::~DirViewer()
{
    delete ui;
}

QDir DirViewer::currentDir()
{
    return dir;
}

void DirViewer::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Enter ||
        event->key() == Qt::Key_Return)
    {
        openCurrentItem();
        event->accept();
        return;
    }

    if (event->key() == Qt::Key_Backspace)
    {
        QDir c = dir;
        c.cdUp();
        on_listView_doubleClicked(model->index(c.absolutePath()));
        event->accept();
        return;
    }
    event->ignore();
}

bool DirViewer::event(QEvent *event)
{
    if (event->type() == OpenDiskList) {
        openDiskList();
        event->accept();
        return true;
    }
    return QWidget::event(event);
}

bool DirViewer::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == selector &&
            event->type() == QEvent::Hide) {
        ui->listView->setFocus();
        return true;
    }
    return QObject::eventFilter(obj, event);
}

void DirViewer::changeDir(QDir dir)
{
    QString path = dir.absolutePath();
    model->setRootPath(path);
    ui->listView->setRootIndex(model->index(path));
    this->dir = dir;
    ui->currentDirectory->setText(QDir::toNativeSeparators(dir.absolutePath()));
    emit dirChanged(dir);
}

void DirViewer::on_listView_doubleClicked(const QModelIndex &index)
{
    QString path = model->filePath(index);
    QFileInfo fi(path);
    if (fi.isDir()){
        changeDir(path);
    } else {
        //emit fileAction(fi);
        ShellExecuteW(NULL, L"open", path.toStdWString().c_str(), NULL,
                      QDir::toNativeSeparators(fi.absoluteDir().absolutePath()).toStdWString().c_str(), SW_SHOWDEFAULT);
    }
}

void DirViewer::openDiskList()
{
    QSize size = selector->size();
    selector->setGeometry(0,0,size.width(), size.height());
    selector->setVisible(true);
}

void DirViewer::directoryLoaded(const QString &path)
{
    Q_UNUSED(path);
    ui->listView->setCurrentIndex(model->index(0));
}

void DirViewer::openCurrentItem()
{
    on_listView_doubleClicked(ui->listView->currentIndex());
}

void DirViewer::onCustomContextMenu(const QPoint &point)
{
    auto index = ui->listView->indexAt(point);
    QPoint gpoint = this->mapToGlobal(point);
    if (index.isValid()) {
        QString path =model->fileInfo(index).absoluteFilePath();
        openShellContextMenuForObject(path.toStdWString(), gpoint.x(), gpoint.y(), (HWND)winId());
    }
}



bool DirViewer::openShellContextMenuForObject(const std::wstring &path, int xPos, int yPos, void * parentWindow)
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
            const int shellOpen = 168;
            if (iCmd == shellOpen) {
                openCurrentItem();
            } else {
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
    }
    DestroyMenu(hMenu);
    return true;
}
