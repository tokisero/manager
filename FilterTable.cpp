#include "FilterTable.h"
#include <QPainter>
#include <QRegularExpression>


FilterTable::FilterTable(Qt::Orientation orientation, QWidget* parent)
    : QHeaderView(orientation, parent)
{
    qDebug() << "FilterTable constructor called";
    filterModel = new QSortFilterProxyModel(this);
    if (orientation == Qt::Horizontal) {
        lineEdits.clear();
        qDebug() << "Horizontal header detected";
    }
}
void FilterTable::updateFilters()
{
    if (filtersCreated) {
        qDebug() << "Filters already created. Skipping creation.";
        return;
    }
    qDebug() << "Updating filters...";
    clearFilters();
    int columnCount = filterModel->sourceModel()->columnCount();
    qDebug() << "Column count: " << columnCount;

    for (int i = 0; i < columnCount; ++i) {
        QLineEdit *lineEdit = new QLineEdit(this);
        lineEdit->setPlaceholderText(filterModel->sourceModel()->headerData(i, Qt::Horizontal).toString());
        lineEdit->setProperty("columnIndex", i);
        connect(lineEdit, &QLineEdit::textChanged, [this, lineEdit](const QString &text) {
            int columnIndex = lineEdit->property("columnIndex").toInt();
            qDebug() << "Filter text changed for column:" << columnIndex << "Text:" << text;
            onFilterTextChanged(columnIndex, text);
        });

        lineEdits.append(lineEdit);
    }

    filtersCreated = true;
}


void FilterTable::clearFilters()
{
    for (auto lineEdit : lineEdits) {
        lineEdit->clear();
        lineEdit->hide();
    }
    lineEdits.clear();

    filtersCreated = false;
}


void FilterTable::resizeEvent(QResizeEvent* event)
{
    QHeaderView::resizeEvent(event);
    if (!filterModel || !filterModel->sourceModel()) {
        qDebug() << "Filter model or source model is null during resize";
        return;
    }

    int columnCount = filterModel->sourceModel()->columnCount();
    int offset = 0;
    for (int i = 0; i < columnCount; i++) {
        QRect sectionRect = calculateSectionRect(i);

        sectionRect.moveLeft(sectionRect.left() + offset);

        qDebug() << "Column" << i << "sectionRect:" << sectionRect;

        if (sectionRect.isValid() && i < lineEdits.size()) {
            int margin = 2;
            lineEdits[i]->setGeometry(sectionRect.x() + margin, sectionRect.y() + margin,
                                      sectionRect.width() - 2 * margin, sectionRect.height() - 2 * margin);
            lineEdits[i]->show();
            qDebug() << "Line edit" << i << "geometry set to:" << lineEdits[i]->geometry();
        }
    }
}

void FilterTable::setFilterModel(QSortFilterProxyModel* proxy)
{
    qDebug() << "setFilterModel called";
    filterModel = proxy;

    if (filterModel && filterModel->sourceModel()) {
        int columnCount = filterModel->sourceModel()->columnCount();
        qDebug() << "Column count in setFilterModel:" << columnCount;

        for (int i = 0; i < columnCount; ++i) {
            QLineEdit* lineEdit = new QLineEdit(this);
            lineEdit->setPlaceholderText(filterModel->sourceModel()->headerData(i, Qt::Horizontal).toString());
            lineEdit->setProperty("columnIndex", i);
            connect(lineEdit, &QLineEdit::textChanged, [this, lineEdit](const QString &text) {
                qDebug() << "Filter text changed for column:" << lineEdit->property("columnIndex").toInt() << "Text:" << text;
                onFilterTextChanged(lineEdit->property("columnIndex").toInt(), text);
            });

            lineEdits.append(lineEdit);
        }
        for (int i = 0; i < columnCount; ++i) {
            if (i < lineEdits.size()) {
                lineEdits[i]->setPlaceholderText(filterModel->sourceModel()->headerData(i, Qt::Horizontal).toString());
                qDebug() << "Setting placeholder for column:" << i << "Text:" << lineEdits[i]->placeholderText();
            }
        }
    }
    else {
        qDebug() << "Filter model or source model is null in setFilterModel";
    }
}




QRect FilterTable::calculateSectionRect(int logicalIndex) const
{
    int pos = sectionViewportPosition(logicalIndex);
    int width = sectionSize(logicalIndex);
    QRect rect(pos, 0, width, height());
    return rect;
}

void FilterTable::paintSection(QPainter* painter, const QRect& rect, int logicalIndex) const
{
    QHeaderView::paintSection(painter, rect, logicalIndex);
}


void FilterTable::onFilterTextChanged(int columnIndex, const QString &text)
{
    qDebug() << "Filter text changed: Column" << columnIndex << "Text:" << text;
    QRegularExpression regex(text, QRegularExpression::CaseInsensitiveOption);
    filterModel->setFilterKeyColumn(columnIndex);
    filterModel->setFilterRegularExpression(regex);
}




QRegularExpression FilterTable::createCombinedFilterExpression(const QStringList &filters)
{
    QStringList regexParts;
    for (const QString &filter : filters) {
        regexParts.append(QRegularExpression::escape(filter));
    }
    QString combinedPattern = regexParts.join(".*");

    qDebug() << "Combined regex pattern:" << combinedPattern;
    return QRegularExpression(combinedPattern, QRegularExpression::CaseInsensitiveOption);
}




void FilterTable::updateTableAndFilters(QSqlTableModel* model)
{
    clearFilters();
    if (model) {
        filterModel->setSourceModel(model);
        model->select();
        qDebug() << "Table model updated and reselected.";
    }
    updateFilters();
}
