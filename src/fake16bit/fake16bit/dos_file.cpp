#include "dos_file.hpp"

#include <cassert>

int dos_file::get_handle( int& p_handle )
{
	assert( m_handles.size() > 0 );

	int handle = m_handles.top();
	m_handles.pop();

	return 0;
}

int dos_file::free_handle(int p_handle)
{
	m_handles.push(p_handle);

	return 0;
}

int dos_file::free_file( int p_handle )
{
	assert( m_handle_map.find( p_handle ) != m_handle_map.end() );

	handle_map_iterator_t i = m_handle_map.find( p_handle );
	m_handle_map.erase( i );

	return 0;
}

FILE* dos_file::get_file( int p_handle )
{
	assert( m_handle_map.find( p_handle ) != m_handle_map.end() );
	return m_handle_map[p_handle];
}

void dos_file::set_file( int p_handle, FILE* p_fp )
{
	m_handle_map[p_handle]=p_fp;
}

void dos_file::fill_stack( int p_stack_size )
{
	m_stack_size = p_stack_size;
	for(int i = 0; i < p_stack_size; ++i )
	{
		m_handles.push(i);
	}
}

dos_file::dos_file()
{
	m_current_handle = 0;

	fill_stack( 0xFFFF );
}

int dos_file::open( std::string& p_filename, int p_mode, int& p_handle )
{
	if( p_mode ){}
	std::string mode = "rw+";

	FILE* fp = fopen( p_filename.c_str(), mode.c_str() );

	get_handle( p_handle );

	set_file( p_handle, fp );

	return 0;
}

int dos_file::close( int p_handle )
{
	FILE* fp = get_file( p_handle );

	fclose(fp);

	free_file( p_handle );
	free_handle( p_handle );

	return 0;
}

int dos_file::seek( int p_handle, int p_offset, int p_origin )
{
	FILE* fp = m_handle_map[p_handle];

	fseek( fp, p_offset, p_origin );

	return 0;
}

int dos_file::unlink( std::string& p_filename )
{
	//delete file
	
	return 0;
}

int dos_file::read( int p_handle /*buffer*/ )
{
	FILE* fp = m_handle_map[p_handle];

	//fread( buffer,1,count, fp );

	return 0;
}

int dos_file::write( int p_handle /*buffer*/ )
{
	FILE* fp = m_handle_map[p_handle];

	//fwrite( buffer,1,count, fp );

	return 0;
}
