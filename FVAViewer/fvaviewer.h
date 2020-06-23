#ifndef FVAVIEWER_H
#define FVAVIEWER_H

#include <QTGui/QDialog>
#include <QTGui/QMainWindow>
#include <QTCore/QtCore>
#include <QTGui/QTreeWidget>

#include "fvacommonlib.h"
#include "FVADescriptionFile.h"

#include "ui_fvaviewer.h"
#include "ui_filters.h"
#include "ui_d-filter.h"
#include "ui_geo-filter.h"
#include "ui_h-filter.h"
#include "ui_e-filter.h"

class QProgressDialog;

/*!
 * \brief keeps main dilaog functionality
 */
class FVAViewer : public QDialog 
{
	Q_OBJECT

	public:

		/*!
		 * \brief ctors-dtors section
		 */
		FVAViewer( const QString& rootDir, const QString& dictPath, QWidget *parent = 0, Qt::WFlags flags = 0 );
		~FVAViewer();

	private: // methods

		/*!
		 * \brief it recursivly expands GUI tree using fva tree
		 * \param fvaitem - fvaitem to populate from
		 * \param item - GUI tree
		 */
		void populateGUITree( const fvaItem* fvaitem, QTreeWidgetItem* item, int& number, QProgressDialog* progress );

		/*!
		 * \brief it recursivly expands FVA tree using file system tree
		 * \param folder - folder path to be populated
		 * \param item - fva tree item
		 */
		void populateFVATree( const QString& folder, fvaItem* fvaitem, int& number, QProgressDialog* progress );

		/*!
		 * \brief it recursivly filters FVA tree
		 * \param filter - filter condition set
		 * \param fvaitem - fva tree item
		 */
		void filterFVATree( const fvaFilter& filter, fvaItem* fvaitem );

		/*!
		 * \brief it recursivly calculates numbers of filesystem tree items
		 * \param folder - folder path to be populated
		 * \param number - number of items
		 */
		void evaluateFSTree (const QString& folder, int& number);

		/*!
		 * \brief it starts showing progress for loading fs tree
		 */
		void showProgress(const QString& rootDir);

		/*!
		 * \brief it initializes gui widgets to show filters
		 */
		void prepareFilters();

	private: // data

		Ui::FVAViewerClass *		m_ui;

		/*!
		 * filters
		 */
		Ui_commonFilter*			m_uiFiltersCommon;
		Ui_dfilter*					m_uiDeviceFilter;
		Ui_placeFilter*				m_uiPlaceFilter;
		Ui_hfilter*					m_uiPeopleFilter;
		Ui_eventFilter*				m_uiEventFilter;

		/*!
		 * root item
		 */
		std::auto_ptr<fvaItem>		m_rootItem;

		/*!
		 * filter condition
		 */
		fvaFilter					m_filter;

#ifdef _NEW_DESC_
		/*!
		 * file with descriptions for FVA items in a folder
		 */
		FVADescriptionFile			m_descriptionFile;
#endif
		/*!
		 * default date-time of filter date-time fields
		  */
		QDateTime					m_defFilterDataTime;

		/*!
		 * global dictionaries
		 */
		QVariantMap					m_dictionaries;

		/*!
		 * path to dictionaries
		 */
		QString						m_dictionaryPath;

		/*!
		 * path to dictionaries
		 */
		FVA_ITEM_MAP				m_fvaItems;

		/*!
		 * icon set to improve performance 
		 */		
		QIcon						m_videoIcon;
		QIcon						m_audioIcon;
		QIcon						m_photoIcon;
		QIcon						m_folderIcon;
		QIcon						m_locationIcon;

	private slots:

		void showItem( QTreeWidgetItem* item );

		void editFileItem( QTreeWidgetItem* item );
		
		void filterClicked(  );

		void ShowContextMenu(const QPoint & point);

		void updateChecks(QTreeWidgetItem*, int);
};

#endif // FVAVIEWER_H