#include "DataModel.h"
USING_NS_CC; 
DataModel* DataModel::m_pInstance;  
DataModel* DataModel::getModel() 
{         
	if (m_pInstance == NULL)
	{       
		m_pInstance = new DataModel();  
	} 
	return m_pInstance; 
}

void DataModel::clear()
{
//    if (GameHUD::_toDeleteHUD != NULL)
//        delete GameHUD::_toDeleteHUD;
//    GameHUD::_toDeleteHUD = GameHUD::_sharHUD;
    GameHUD::_sharHUD = NULL;
    delete m_pInstance;
    m_pInstance = NULL;
}