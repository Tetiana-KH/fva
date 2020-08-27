#ifndef FVAITEM_H
#define FVAITEM_H

/*!
 * \brief it keeps whole information about a foto-video-audio element
 */
class fvaItem
{
	public : 

		fvaItem ();
		virtual ~fvaItem ();

		/*!
		 * \brief it returns human-readable string as item name
		 * \param dictionaries - global dictionary set
		 */
		QString getGuiName(const QVariantMap&	dictionaries);

		/*!
		 * \brief it returns human-readable string to show full name
		 * \param dictionaries - global dictionary set
		 */
		QString getGuiFullName(const QVariantMap&	dictionaries);

	public : // data

		/*!
		 * fs type
		 */		
		FVA_FS_TYPE				type;

		/*!
		 * for folder it is event date period, for file it is date taken(dateFrom only)
		 * both are filesystem names
		 */
		QDateTime				dateFrom;
		QDateTime				dateTo;

		/*!
		 * children
		 */
		std::list<fvaItem*>		children;

		/*!
		 * absolute path to item
		 */
		QString					fsFullPath;

		/*!
		 * does this item fit filtration conditions
		 */
		bool					isFiltered;
		
		/*!
		 * folder fva specific info
		 */
		fvaFolder*				fvaFolder;

		/*!
		 * file fva specific info
		 */
		fvaFile*				fvaFile;
};

#endif //FVAITEM_H