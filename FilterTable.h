#ifndef FILTERTABLE_H
#define FILTERTABLE_H

#include <QHeaderView>
#include <QLineEdit>
#include <QSortFilterProxyModel>
#include <QSqlTableModel>

class FilterTable : public QHeaderView
{
    Q_OBJECT

public:
    explicit FilterTable(Qt::Orientation orientation, QWidget* parent = nullptr);
    void setFilterModel(QSortFilterProxyModel* proxy);
    void updateTableAndFilters(QSqlTableModel* model);
    void clearFilters();
    void updateFilters();
    QList<QLineEdit*> getLineEdits() const { return lineEdits; };
    bool filtersCreated = false;

protected:
    void resizeEvent(QResizeEvent* event) override;
    void paintSection(QPainter* painter, const QRect& rect, int logicalIndex) const override;

private slots:
    void onFilterTextChanged(int columnIndex, const QString &text);

private:
    QRect calculateSectionRect(int logicalIndex) const;
    QVector<QLineEdit*> lineEdits;
    QSortFilterProxyModel* filterModel = nullptr;
    QRegularExpression createCombinedFilterExpression(const QStringList& filters);


};

#endif // FILTERTABLE_H


