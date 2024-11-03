#ifndef _CASERV_SERVICE_MODELS_H_
#define _CASERV_SERVICE_MODELS_H_

#include <ctime>
#include <memory>
#include <string>

#include "./../../common/datetime.h"
#include "./../../contracts/enums.h"
#include "./../../libs/json.hpp"

namespace service {
namespace models {

using namespace datetime;
using namespace contracts;

struct CreateCertificateAuthorityModel {
  AlgorithmEnum algorithm{AlgorithmEnum::GostR3410_2012_256};
  std::string commonName;
  std::string country{"RU"};
  std::string localityName;
  std::string stateOrProvinceName;
  std::string streetAddress;
  std::string emailAddress;
  std::string innLe;
  std::string ogrn;
  std::string organizationName;
  uint16_t ttlInDays{365};
  std::string publicUrl;
};

struct StoredCertificateAuthorityModel {
  std::string serial;
  std::string thumbprint;
  std::string commonName;
  DateTimePtr issueDate;
  std::string publicUrl;
};

struct StoredCertificateModel {
  std::string serial;
  std::string thumbprint;
  std::string caSerial;
  std::string commonName;
  DateTimePtr issueDate;
  DateTimePtr revokeDate;
};

struct IssueCertificateModel {
  SujectTypeEnum subjectType{SujectTypeEnum::PhysicalPerson};
  AlgorithmEnum algorithm{AlgorithmEnum::GostR3410_2012_256};
  uint16_t ttlInDays{365};
  std::string commonName;
  std::string country{"RU"};
  std::string localityName;
  std::string stateOrProvinceName;
  std::string streetAddress;
  std::string emailAddress;
  std::string inn;
  std::string snils;
  std::string givenName;
  std::string surname;
  std::string ogrnip;
  std::string innLe;
  std::string ogrn;
  std::string organizationName;
  std::string organizationUnitName;
  std::string title;
};

using CreateCertificateAuthorityModelPtr =
    std::shared_ptr<CreateCertificateAuthorityModel>;
using StoredCertificateAuthorityModelPtr =
    std::shared_ptr<StoredCertificateAuthorityModel>;
using StoredCertificateModelPtr = std::shared_ptr<StoredCertificateModel>;
using IssueCertificateModelPtr = std::shared_ptr<IssueCertificateModel>;

// TODO move to separated files
using json = nlohmann::json;

inline void from_json(const json &json, IssueCertificateModelPtr &model) {
  json.at("subjectType").get_to(model->subjectType);
  json.at("algorithm").get_to(model->algorithm);
  json.at("ttlInDays").get_to(model->ttlInDays);
  json.at("commonName").get_to(model->commonName);
  json.at("country").get_to(model->country);
  json.at("localityName").get_to(model->localityName);
  json.at("stateOrProvinceName").get_to(model->stateOrProvinceName);
  json.at("streetAddress").get_to(model->streetAddress);
  json.at("emailAddress").get_to(model->emailAddress);
  json.at("inn").get_to(model->inn);
  json.at("snils").get_to(model->snils);
  json.at("givenName").get_to(model->givenName);
  json.at("surname").get_to(model->surname);
  json.at("ogrnip").get_to(model->ogrnip);
  json.at("innLe").get_to(model->innLe);
  json.at("ogrn").get_to(model->ogrn);
  json.at("organizationName").get_to(model->organizationName);
  json.at("organizationUnitName").get_to(model->organizationUnitName);
  json.at("title").get_to(model->title);
}

inline void to_json(json &j,
                    const service::models::StoredCertificateModelPtr &model) {
  j["serial"] = model->serial;
  j["thumbprint"] = model->thumbprint;
  j["caSerial"] = model->caSerial;
  j["commonName"] = model->commonName;
  j["issueDate"] = datetime::to_utcstring(model->issueDate);
  if (model->revokeDate != nullptr)
    j["revokeDate"] = datetime::to_utcstring(model->revokeDate);
}

inline void
to_json(json &j,
        const service::models::StoredCertificateAuthorityModelPtr &model) {
  j["serial"] = model->serial;
  j["thumbprint"] = model->thumbprint;
  j["commonName"] = model->commonName;
  j["issueDate"] = datetime::to_utcstring(model->issueDate);
  j["publicUrl"] = model->publicUrl;
}

} // namespace models
} // namespace service

#endif //_CASERV_SERVICE_MODELS_H_
