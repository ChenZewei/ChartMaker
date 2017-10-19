#include "xml.h"

XML::XML()
{
	output = new XMLDocument();
}

XMLDocument XML::config;

void XML::LoadFile(const char* path)
{
	config.LoadFile(path);
}

void XML::SaveConfig(const char* path)
{
	config.SaveFile(path);
}

void XML::get_method(Test_Attribute_Set *t_set)
{
	const char* content;
	XMLElement *root = config.RootElement();
	XMLElement *title = root->FirstChildElement("schedulability_test");
	XMLElement *subtitle = title->FirstChildElement("data");
	while(subtitle)
	{
		content = subtitle->GetText();
//cout<<content<<endl;
		Test_Attribute ta;
		int test_type = subtitle->IntAttribute("TEST_TYPE");
		string remark;
		string rename;
		string style;
		uint cluster;
		if(NULL == subtitle->Attribute("REMARK"))
			remark = "";
		else
			remark = subtitle->Attribute("REMARK");
		if(NULL == subtitle->Attribute("RENAME"))
			rename = "";
		else
			rename = subtitle->Attribute("RENAME");
		if(NULL == subtitle->Attribute("STYLE"))
			style = "";
		else
			style = subtitle->Attribute("STYLE");
		if(NULL == subtitle->Attribute("CLUSTER"))
			cluster = 0;
		else
			cluster = atoi(subtitle->Attribute("CLUSTER"));
		int transform = 0;
		if(0 == strcmp(content,"P-EDF"))
		{
			transform = 0;
		}
		else if(0 == strcmp(content,"BCL-FTP"))
		{
			transform = 1;
		}
		else if(0 == strcmp(content,"BCL-EDF"))
		{
			transform = 2;
		}
		else if(0 == strcmp(content,"WF-DM"))
		{
			transform = 3;
		}
		else if(0 == strcmp(content,"WF-EDF"))
		{
			transform = 4;
		}
		else if(0 == strcmp(content,"RTA-GFP"))
		{
			transform = 5;
		}
		else if(0 == strcmp(content,"FF-DM"))
		{
			transform = 6;
		}
		else if(0 == strcmp(content,"LP-PFP"))
		{
			transform = 7;
		}
		else if(0 == strcmp(content,"LP-GFP"))
		{
			transform = 8;
		}
		else if(0 == strcmp(content,"RO-PFP"))
		{
			transform = 9;
		}
		else if(0 == strcmp(content,"ILP-SPINLOCK"))
		{
			transform = 10;
		}
		else if(0 == strcmp(content,"GEDF-NON-PREEMPT"))
		{
			transform = 11;
		}
		else if(0 == strcmp(content,"PFP-GS"))
		{
			transform = 12;
		}
		else
		{
			transform = 0;
		}
		ta.test_method = transform;
		ta.test_type = test_type;
		ta.test_name = content;
		ta.remark = remark;
		ta.rename = rename;
		ta.style = style;
		ta.cluster = cluster;
		t_set->push_back(ta);
//		i_set->push_back(transform);
		subtitle = subtitle->NextSiblingElement();
	}
}

uint XML::get_experiment_times()
{
	const char* content;
	XMLElement *root = config.RootElement();
	XMLElement *title = root->FirstChildElement("experiment_times");
	return atoi(title->GetText());
}

void XML::get_lambda(Int_Set *i_set)
{
	const char* content;
	XMLElement *root = config.RootElement();
	XMLElement *title = root->FirstChildElement("lambda");
	XMLElement *subtitle = title->FirstChildElement("data");
	while(subtitle)
	{
		content = subtitle->GetText();
		i_set->push_back(atoi(content));
		subtitle = subtitle->NextSiblingElement();
	}
}

void XML::get_period_range(Range_Set *r_set)
{
	const char* content;
	XMLElement *root = config.RootElement();
	XMLElement *title = root->FirstChildElement("period_range");
	XMLElement *subtitle = title->FirstChildElement("data");
	while(subtitle)
	{
		Range temp;
		XMLElement *SSubtitle = subtitle->FirstChildElement("min");
		content = SSubtitle->GetText();
		fraction_t data(content);
		temp.min = data.get_d();
		SSubtitle = subtitle->FirstChildElement("max");
		content = SSubtitle->GetText();
		data = content;
		temp.max = data.get_d();
		r_set->push_back(temp);
		subtitle = subtitle->NextSiblingElement();
	}
}

void XML::get_deadline_propotion(Range_Set *r_set)
{
	const char* content;
	XMLElement *root = config.RootElement();
	XMLElement *title = root->FirstChildElement("deadline_propotion");
	XMLElement *subtitle = title->FirstChildElement("data");
	while(subtitle)
	{
		Range temp;
		XMLElement *SSubtitle = subtitle->FirstChildElement("min");
		content = SSubtitle->GetText();
		floating_t data(content);
		temp.min = data.get_d();
		SSubtitle = subtitle->FirstChildElement("max");
		content = SSubtitle->GetText();
		data = content;
		temp.max = data.get_d();
		r_set->push_back(temp);
		subtitle = subtitle->NextSiblingElement();
	}
}

void XML::get_utilization_range(Range_Set *r_set)
{
	const char* content;
	XMLElement *root = config.RootElement();
	XMLElement *title = root->FirstChildElement("init_utilization_range");
	XMLElement *subtitle = title->FirstChildElement("data");
	while(subtitle)
	{
		Range temp;
		XMLElement *SSubtitle = subtitle->FirstChildElement("min");
		content = SSubtitle->GetText();
		floating_t data(content);
		temp.min = data.get_d();
		SSubtitle = subtitle->FirstChildElement("max");
		content = SSubtitle->GetText();
		data = content;
		temp.max = data.get_d();
		r_set->push_back(temp);
		subtitle = subtitle->NextSiblingElement();
	}
}

void XML::get_step(Double_Set *d_set)
{
	const char* content;
	XMLElement *root = config.RootElement();
	XMLElement *title = root->FirstChildElement("step");
	XMLElement *subtitle = title->FirstChildElement("data");
	while(subtitle)
	{
		content = subtitle->GetText();
		floating_t temp(content);
		d_set->push_back(temp.get_d());
		subtitle = subtitle->NextSiblingElement();
	}
}

void XML::get_processor_num(Int_Set *i_set)
{
	const char* content;
	XMLElement *root = config.RootElement();
	XMLElement *title = root->FirstChildElement("processor_num");
	XMLElement *subtitle = title->FirstChildElement("data");
	while(subtitle)
	{
		content = subtitle->GetText();
		i_set->push_back(atoi(content));
		subtitle = subtitle->NextSiblingElement();
	}
}

//defalut

void XML::get_integers(Int_Set *i_set, const char* element_name)
{
	const char* content;
	XMLElement *root = config.RootElement();
	XMLElement *title = root->FirstChildElement(element_name);
	XMLElement *subtitle = title->FirstChildElement("data");
	while(subtitle)
	{
		content = subtitle->GetText();
		i_set->push_back(atoi(content));
		subtitle = subtitle->NextSiblingElement();
	}
}

void XML::get_doubles(Double_Set *d_set, const char* element_name)
{
	const char* content;
	XMLElement *root = config.RootElement();
	XMLElement *title = root->FirstChildElement(element_name);
	XMLElement *subtitle = title->FirstChildElement("data");
	while(subtitle)
	{
		content = subtitle->GetText();
		floating_t temp(content);
		d_set->push_back(temp.get_d());
		subtitle = subtitle->NextSiblingElement();
	}
}

void XML::get_ranges(Range_Set *r_set, const char* element_name)
{
	const char* content;
	XMLElement *root = config.RootElement();
	XMLElement *title = root->FirstChildElement(element_name);
	XMLElement *subtitle = title->FirstChildElement("data");
	while(subtitle)
	{
		Range temp;
		XMLElement *SSubtitle = subtitle->FirstChildElement("min");
		content = SSubtitle->GetText();
		floating_t data(content);
		temp.min = data.get_d();
		SSubtitle = subtitle->FirstChildElement("max");
		content = SSubtitle->GetText();
		data = content;
		temp.max = data.get_d();
		r_set->push_back(temp);
		subtitle = subtitle->NextSiblingElement();
	}
}

//resource

void XML::get_resource_num(Int_Set *i_set)
{
	const char* content;
	XMLElement *root = config.RootElement();
	XMLElement *title = root->FirstChildElement("resource_num");
	XMLElement *subtitle = title->FirstChildElement("data");
	while(subtitle)
	{
		content = subtitle->GetText();
		i_set->push_back(atoi(content));
		subtitle = subtitle->NextSiblingElement();
	}
}

//resource request

void XML::get_resource_request_probability(Double_Set *d_set)
{
	const char* content;
	XMLElement *root = config.RootElement();
	XMLElement *title = root->FirstChildElement("rrp");
	XMLElement *subtitle = title->FirstChildElement("data");
	while(subtitle)
	{
		content = subtitle->GetText();
		floating_t temp(content);
		d_set->push_back(temp.get_d());
		subtitle = subtitle->NextSiblingElement();
	}
}

void XML::get_resource_request_num(Int_Set *i_set)
{
	const char* content;
	XMLElement *root = config.RootElement();
	XMLElement *title = root->FirstChildElement("rrn");
	XMLElement *subtitle = title->FirstChildElement("data");
	while(subtitle)
	{
		content = subtitle->GetText();
		i_set->push_back(atoi(content));
		subtitle = subtitle->NextSiblingElement();
	}
}

void XML::get_resource_request_range(Range_Set *r_set)
{
	const char* content;
	XMLElement *root = config.RootElement();
	XMLElement *title = root->FirstChildElement("rrr");
	XMLElement *subtitle = title->FirstChildElement("data");
	while(subtitle)
	{
		Range temp;
		XMLElement *SSubtitle = subtitle->FirstChildElement("min");
		content = SSubtitle->GetText();
		floating_t data(content);
		temp.min = data.get_d();
		SSubtitle = subtitle->FirstChildElement("max");
		content = SSubtitle->GetText();
		data = content;
		temp.max = data.get_d();
		r_set->push_back(temp);
		subtitle = subtitle->NextSiblingElement();
	}
}

void XML::get_total_len_factor(Double_Set *d_set)
{
	const char* content;
	XMLElement *root = config.RootElement();
	XMLElement *title = root->FirstChildElement("total_len_factor");
	XMLElement *subtitle = title->FirstChildElement("data");
	while(subtitle)
	{
		content = subtitle->GetText();
		floating_t temp(content);
		d_set->push_back(temp.get_d());
		subtitle = subtitle->NextSiblingElement();
	}
}

//xml construction
void XML::initialization()
{
	XMLDeclaration *declaration = output->NewDeclaration();
	output->InsertFirstChild(declaration);
}

void XML::add_element(const char* name)
{
	XMLElement *root = output->NewElement(name);
	root->SetName(name);
	output->InsertEndChild(root);
}

void XML::add_element(const char* parent, const char* name, const char* text)
{
	XMLElement *root = output->RootElement();
	XMLElement *title = output->FirstChildElement(parent);
	XMLElement *element = output->NewElement(name);
	element->SetText(text);
	title->InsertEndChild(element);
}

void XML::add_range(const char* parent, Range range)
{
	XMLElement *root = output->RootElement();
	XMLElement *title = root->FirstChildElement(parent);
	XMLElement *data = output->NewElement("data");
	XMLElement *min = output->NewElement("min");
	XMLElement *max = output->NewElement("max");
	fraction_t transform = range.min;
	min->SetText(transform.get_str().data());
	data->InsertEndChild(min);
	transform = range.max;
	max->SetText(transform.get_str().data());
	data->InsertEndChild(max);
	title->InsertFirstChild(data);
}

void XML::set_text(const char* parent, int index1, const char* element, int index2,const char* text)
{
	XMLElement *root = output->RootElement();
	XMLElement *title = output->FirstChildElement(parent);
	for(int i = 0; i < index1; i++)
		title = title->NextSiblingElement();
	XMLElement *subtitle = title->FirstChildElement(element);
	for(int i = 0; i < index2; i++)
		subtitle = subtitle->NextSiblingElement();
	subtitle->SetText(text);
}

void XML::save_file(const char* path)
{
	output->SaveFile(path);
}

void XML::clear()
{
	output->Clear();
}