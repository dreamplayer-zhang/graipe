/************************************************************************/
/*                                                                      */
/*               Copyright 2008-2017 by Benjamin Seppke                 */
/*       Cognitive Systems Group, University of Hamburg, Germany        */
/*                                                                      */
/*    This file is part of the GrAphical Image Processing Enviroment.   */
/*    The GRAIPE Website may be found at:                               */
/*        https://github.com/bseppke/graipe                             */
/*    Please direct questions, bug reports, and contributions to        */
/*    the GitHub page and use the methods provided there.               */
/*                                                                      */
/*    Permission is hereby granted, free of charge, to any person       */
/*    obtaining a copy of this software and associated documentation    */
/*    files (the "Software"), to deal in the Software without           */
/*    restriction, including without limitation the rights to use,      */
/*    copy, modify, merge, publish, distribute, sublicense, and/or      */
/*    sell copies of the Software, and to permit persons to whom the    */
/*    Software is furnished to do so, subject to the following          */
/*    conditions:                                                       */
/*                                                                      */
/*    The above copyright notice and this permission notice shall be    */
/*    included in all copies or substantial portions of the             */
/*    Software.                                                         */
/*                                                                      */
/*    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND    */
/*    EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES   */
/*    OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND          */
/*    NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT       */
/*    HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,      */
/*    WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING      */
/*    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR     */
/*    OTHER DEALINGS IN THE SOFTWARE.                                   */
/*                                                                      */
/************************************************************************/

#include "core/serializable.hxx"

//image type
#include "vectorfields/densevectorfieldimpex.hxx"

namespace graipe {

/**
 * @addtogroup graipe_vectorfields
 * @{
 *     @file
 *     @brief Implementation file for dense vectorfield import and export
 * @}
 */

bool DenseVectorfieldImpex::importVectorfield(const QString & filename, DenseVectorfield2D & vf)
{
    //The flo-format:
    //  bytes  contents
    //
    //  0-3     tag: "PIEH" in ASCII, which in little endian happens to be the float 202021.25
    //          (just a sanity check that floats are represented correctly)
    //  4-7     width as an integer
    //  8-11    height as an integer
    //  12-end  data (width*height*2*4 bytes total)
    //          the float values for u and v, interleaved, in row order, i.e.,
    //          u[row0,col0], v[row0,col0], u[row0,col1], v[row0,col1], ...
    //
    try
    {
        if(!filename.isEmpty())
        {
            QFile file(filename);
            
            if(file.open(QIODevice::ReadOnly))
            {
                //Check header:
                const QByteArray text = file.read(4);
            
                if(QString(text) == "PIEH")
                {
                    int width = 0;
                    int height = 0;
                    
                    if( file.read((char*)&width, 4) != 4)
                        return false;
                    
                    if( file.read((char*)&height, 4) != 4)
                        return false;
                 
                    if(width == 0 || height == 0)
                        return false;
                    
                    vf.clear();
                    
                    vf.setTop(0);
                    vf.setLeft(0);
                    vf.setBottom(height);
                    vf.setRight(width);
                    
                    DenseVectorfield2D::ArrayType u(width, height);
                    DenseVectorfield2D::ArrayType v(width, height);
                    
                    float u_val = 0;
                    float v_val = 0;
                    
                    for(int y=0; y!=height; ++y)
                    {
                        for(int x=0; x!=width; ++x)
                        {
                            if( file.read((char*)&u_val, 4) != 4)
                                return false;
                            
                            u(x,y) = u_val;
                            
                            if( file.read((char*)&v_val, 4) != 4)
                                return false;
                            
                            v(x,y) = v_val;
                        }
                    }
                    file.close();
                    
                    vf.setU(u);
                    vf.setV(v);
                    
                    return true;
                }
                else
                {
                    file.close();
                }
            }
        }
    }
    catch (...)
    {
        qInfo() << "Error: File: " << filename << " was not loaded into dense vectorfield\n";

    }
    return false;
}

bool DenseVectorfieldImpex::exportVectorfield(const DenseVectorfield2D & vf, const QString& filename)
{
       //The flo-format:
    //  bytes  contents
    //
    //  0-3     tag: "PIEH" in ASCII, which in little endian happens to be the float 202021.25
    //          (just a sanity check that floats are represented correctly)
    //  4-7     width as an integer
    //  8-11    height as an integer
    //  12-end  data (width*height*2*4 bytes total)
    //          the float values for u and v, interleaved, in row order, i.e.,
    //          u[row0,col0], v[row0,col0], u[row0,col1], v[row0,col1], ...
    //
    try
    {
        if(!filename.isEmpty())
        {
            QFile file(filename);
            
            if(file.open(QIODevice::WriteOnly))
            {
                //Write header:
                if( file.write("PIEH", 4) != 4)
                    return false;
                
                int width = vf.width();
                int height = vf.height();
                
                //Write dimensions
                if( file.write((char*)&width, 4) != 4)
                    return false;
                    
                if( file.write((char*)&height, 4) != 4)
                    return false;
                 
                float u_val = 0;
                float v_val = 0;
                    
                for(int y=0; y!=height; ++y)
                {
                    for(int x=0; x!=width; ++x)
                    {
                        u_val = vf.u()(x,y);
                        
                        if( file.write((char*)&u_val, 4) != 4)
                            return false;
                            
                        v_val = vf.v()(x,y);
                            
                        if( file.write((char*)&v_val, 4) != 4)
                            return false;
                            
                    }
                }
                file.close();
                return true;
            }
            else
            {
                file.close();
            }
        }
    }
    catch (...)
    {
        qInfo() << "Error: File: " << filename << " was not saved from dense vectorfield\n";

    }
    return false;
}


DenseVectorfieldImporter::DenseVectorfieldImporter(Workspace* wsp)
: Algorithm(wsp)
{
    m_parameters->addParameter("filename", new FilenameParameter(".flo filename", "", NULL));
    m_results.push_back(new DenseVectorfield2D(wsp));
}

void DenseVectorfieldImporter::run()
{
    try 
    {
        emit statusMessage(0.0, QString("started"));
        
        FilenameParameter	* param_filename = static_cast<FilenameParameter*> ((*m_parameters)["filename"]);
        
        QString filename = param_filename->value();
        
        if(DenseVectorfieldImpex::importVectorfield(filename, *static_cast<DenseVectorfield2D*>(m_results[0])))
        {
            emit statusMessage(100.0, QString("finished computation"));
            emit finished();
        }
        else 
        {
            emit errorMessage(QString("Explainable error occured: Dense vectorfield could not be imported"));
        }
    }
    catch(std::exception& e)
    {
        emit errorMessage(QString("Explainable error occured: ") + QString::fromStdString(e.what()));
    }
    catch(...)
    {
        emit errorMessage(QString("Non-explainable error occured"));
    }
}

DenseVectorfieldExporter::DenseVectorfieldExporter(Workspace* wsp)
: Algorithm(wsp)
{
    m_parameters->addParameter("vectorfield", new ModelParameter("Vectorfield",	"DenseVectorfield2D, DenseWeightedVectorfield2D", NULL, false, wsp));
    m_parameters->addParameter("filename", new FilenameParameter(".flo filename", "", NULL));
}

void DenseVectorfieldExporter::run()
{
    try 
    {
        emit statusMessage(0.0, QString("started"));
        
        ModelParameter		* param_vectorfield      = static_cast<ModelParameter*> ((*m_parameters)["Vectorfield"]);
        FilenameParameter	* param_filename = static_cast<FilenameParameter*> ((*m_parameters)["filename"]);
        
        if(DenseVectorfieldImpex::exportVectorfield(*static_cast<DenseVectorfield2D*>(param_vectorfield->value()),
                                          param_filename->value()))
        {
            emit statusMessage(100.0, QString("finished computation"));
            emit finished();				
        }
        else {
            emit errorMessage(QString("Explainable error occured: Dense vectorfield could not be exported!"));
        }

        
    }
    catch(std::exception& e)
    {
        emit errorMessage(QString("Explainable error occured: ") + QString::fromStdString(e.what()));
    }
    catch(...)
    {
        emit errorMessage(QString("Non-explainable error occured"));		
    }
}

} //end of namespace graipe
