#include "DBModule.h"

DBModule::DBModule()
{

}

DBModule::~DBModule()
{

}

//��ʼ�����ݿ�
void DBModule::initDB()
{
	//�ж����ݿ��ļ��Ƿ����
	bool create = !QFile::exists("CZPlayer.sqlite");

	if (!DBModule::createConnection())
	{
		QMessageBox::warning(0, tr("��ʾ"), tr("�������ݿ����!"));
		return;
	}
	if (create)
	{
		DBModule::createTable();
	}
}

//�������ݿ�
bool DBModule::createConnection()
{
	QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
	db.setDatabaseName("CZPlayer.sqlite");//�����ݿ��ļ������ڣ����Զ�����һ��

	if (!db.open())
	{
		QMessageBox::warning(0, tr("���ݿ�򿪴���!"),
			db.lastError().databaseText());
		return false;
	}
	return true;
}

//���û�д������򴴽�
void DBModule::createTable()
{
	QSqlQuery query;
	query.exec("drop table musicList");

	query.exec("create table musicList (fileName varchar(50) primary key not null, "
		"time varchar(10), filePath varchar(50), albumUrlPath varchar(100))");
}

//�����ݿ��ж��������ļ���Ϣ�������ֲ����б�
bool DBModule::readFilePath( QStringList &list )
{
	QSqlQuery query;
	query.exec("select filePath from musicList");

	while (query.next())
	{
		QString filePath = query.value(0).toString();
		list << filePath;
	}

	if (!list.isEmpty())
	{
		return true;
	}

	return false;
}

//�����ݿ��ж���������ʾ��m3u�ļ���
bool DBModule::readMusicListSaveFormatEntity( vector<MusicListSaveFormatEntity> &vec )
{
	QSqlQuery query;
	query.exec("select fileName, time, filePath from musicList");

	while (query.next())
	{
		MusicListSaveFormatEntity entity(query.value(0).toString().toStdString(),
									query.value(1).toString().toStdString(),
									query.value(2).toString().toStdString());
		vec.push_back(entity);
	}

	if (!vec.empty())
	{
		return true;
	}

	return false;
}

//������ݿ�
bool DBModule::clearDB()
{
	QSqlQuery query;
	return query.exec("delete from musicList");
}

//ɾ��һ����¼
bool DBModule::delLine( const QString &fileName )
{
	QSqlQuery query;
	query.prepare("delete from musicList where fileName = ?");
	query.addBindValue(fileName);
	return query.exec();
}

//����һ�м�¼
bool DBModule::insertLine( const QString &fileName, const QString &time, const QString &filePath )
{
	QSqlQuery query;
	query.prepare("insert into musicList values(?, ?, ?, ?)");
	query.addBindValue(fileName);
	query.addBindValue(time);
	query.addBindValue(filePath);
	query.addBindValue("");
	return query.exec();
}

//����ר��url
bool DBModule::updateAlbumUrlPath( const QString &fileName, const QString &albumUrlPath )
{
	QSqlQuery query;
	query.prepare("update musicList set albumUrlPath = ? where fileName = ?");
	query.addBindValue(albumUrlPath);
	query.addBindValue(fileName);
	return query.exec();
}

//�����ݿ��ж�����������Ӧ��ר��url
bool DBModule::readAlbumUrlPathFromDB( QString &fileName, QString &albumUrlPath )
{
	QSqlQuery query;
	query.prepare("select albumUrlPath from musicList where fileName = ?");
	query.addBindValue(fileName);
	query.exec();

	while (query.next())
	{
		albumUrlPath = query.value(0).toString();
		break;
	}

	return true;
}

//�����ݿ��ȡ������Ϣ
bool DBModule::readMusicInfoEntity( vector<MusicInfoEntity> &vec )
{
	QSqlQuery query;
	query.exec("select fileName, time, filePath from musicList");

	while (query.next())
	{
		MusicInfoEntity entity(query.value(0).toString().toStdString(),
			query.value(1).toString().toStdString(),
			query.value(2).toString().toStdString());
		vec.push_back(entity);
	}

	if (!vec.empty())
	{
		return true;
	}

	return false;
}