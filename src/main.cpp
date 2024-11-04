﻿// caserver.cpp : Defines the entry point for the application.
//

#include <cstdio>
#include <cstring>
#include <ctime>
#include <httpserver.hpp>
#include <iostream>
#include <memory>
#include <openssl/bio.h>
#include <openssl/bn.h>
#include <openssl/crypto.h>
#include <openssl/engine.h>
#include <openssl/err.h>
#include <openssl/evp.h>
#include <openssl/txt_db.h>
#include <ostream>
#include <spdlog/common.h>
#include <spdlog/spdlog.h>
#include <string_view>
#include <utility>
#include <vector>

#include "base/icrypto_provider.h"
#include "contracts/certificate_request.h"
#include "contracts/enums.h"
#include "http/get_ca.h"
#include "http/get_ca_certificate.h"
#include "http/get_certificate.h"
#include "http/get_certificates.h"
#include "http/get_crl.h"
#include "http/get_crt.h"
#include "http/post_issue_certificate.h"
#include "openssl/crypto_provider.h"
#include "postgre/pgdatabase.h"
#include "service/caservice.h"

using namespace std;

contracts::JuridicalPersonCertificateRequest caReq;
contracts::JuridicalPersonCertificateRequest clientReq;

int main() {

  try {
    // OPENSSL_add_all_algorithms_conf();
    // OPENSSL_init_crypto(uint64_t opts, const OPENSSL_INIT_SETTINGS *settings)
    spdlog::set_level(spdlog::level::level_enum::debug);
    // openssl::Provider provider{nullptr};
    openssl::OpensslCryptoProvider provider;

    contracts::JuridicalPersonCertificateRequest caReq;
    caReq.commonName = "ООО Очень Тестовый УЦ";
    caReq.country = "RU";
    caReq.stateOrProvinceName = "78 г.Санкт-Петербург";
    caReq.localityName = "Санкт-Петербург";
    caReq.streetAddress = "ул. Большая Морская";
    caReq.emailAddress = "test@testemail.ru";
    caReq.innLe = "1234567890";
    caReq.ogrn = "1234567890123";
    caReq.organizationName = "ООО Очень Тестовый УЦ";
    caReq.organizationUnitName = "Отдел фейковых выдач";
    caReq.algorithm = contracts::AlgorithmEnum::GostR3410_2012_512;

    contracts::JuridicalPersonCertificateRequest clientReq;
    clientReq.commonName = "ООО Рога и Копыта";
    clientReq.country = "RU";
    clientReq.stateOrProvinceName = "78 г.Санкт-Петербург";
    clientReq.localityName = "Санкт-Петербург";
    clientReq.streetAddress = "ул. Пушкина";
    clientReq.emailAddress = "test@testemail.ru";
    clientReq.inn = "123456789012";
    clientReq.givenName = "Иван Иванович";
    clientReq.surname = "Иванов";
    clientReq.snils = "12334536322";
    clientReq.innLe = "2234467890";
    clientReq.ogrn = "2224567890123";
    clientReq.organizationName = "ООО Рога и Копыта";
    clientReq.organizationUnitName = "Директорат";
    clientReq.title = "Предводитель";
    clientReq.algorithm = contracts::AlgorithmEnum::GostR3410_2012_256;

    std::vector<std::string_view> crlDistributionPoints{
        "http://test.ru/crl.crl"};

    std::vector<std::string_view> ocspEndPoints{};

    std::vector<std::string_view> caEndPoints{"http://test.ru/root.crt"};

    // auto root = provider.GeneratedCACertificate(caReq);

    // contracts::CaInfo caInfo {
    //   .crlDistributionPoints = crlDistributionPoints,
    //   .ocspEndPoints = ocspEndPoints,
    //   .caEndPoints = caEndPoints,
    //   .privateKey = root->privateKey,
    //   .certificate = root->certificate
    // };

    // auto client = provider.GenerateClientCertitificate(clientReq, caInfo);
    // LOG_INFO(client->serialNumber.data());
    // LOG_INFO(client->thumbprint);

    // auto client = provider.GenerateClientCertitificate(clientReq, issuerCert,
    // issuerKey); print(client); auto crl = provider.CreateCRL(issuerCert,
    // issuerKey, std::vector<X509*>{client.first.get()});

    // postrgre::PgDatabase db(connString);

    service::models::CreateCertificateAuthorityModel createCaModel;
    createCaModel.commonName = "ООО Очень Тестовый УЦ";
    createCaModel.country = "RU";
    createCaModel.stateOrProvinceName = "78 г.Санкт-Петербург";
    createCaModel.localityName = "Санкт-Петербург";
    createCaModel.streetAddress = "ул. Большая Морская";
    createCaModel.emailAddress = "test@testemail.ru";
    createCaModel.innLe = "1234567890";
    createCaModel.ogrn = "1234567890123";
    createCaModel.organizationName = "ООО Очень Тестовый УЦ";
    createCaModel.algorithm = contracts::AlgorithmEnum::GostR3410_2012_512;
    createCaModel.publicUrl = "http://testca:8080";

    auto connString = "postgresql://admin:admin@127.0.0.1:5432/postgres";
    db::IDataBasePtr db = std::make_shared<postgre::PgDatabase>(connString);
    base::ICryptoProviderUPtr crypt =
        std::make_unique<openssl::OpensslCryptoProvider>();
    auto caService = std::make_shared<serivce::CaService>(db, std::move(crypt));
    // caService->CreateCA(createCaModel);

    // auto client = caService->CreateClientCertificate(
    //     "E5767690E57697D703D8F3F21F047FC2", clientReq);
    // std::ofstream file;
    // file.open("test.pfx", std::ios::out | std::ios::binary);
    // file.write(reinterpret_cast<const char*>(client->container.data()),
    // client->container.size()); file.close();

    httpserver::webserver ws = httpserver::create_webserver(8080);
    http::GetCrlEndpoint getCrl(caService);
    http::GetCrtEndpoint getCrt(caService);
    http::GetCertificateEndpoint getCertificate(caService);
    http::GetCertificatesEndpoint getCertificates(caService);
    http::GetCaEndpoint getCa(caService);
    http::GetCaCertificateEndpoint getCaCert(caService);
    http::IssuePhysicalPersonEndpoint issueCert(caService);

    ws.register_resource(getCrl.Route(), &getCrl);
    ws.register_resource(getCrt.Route(), &getCrt);
    ws.register_resource(getCertificate.Route(), &getCertificate);
    ws.register_resource(getCertificates.Route(), &getCertificates);
    ws.register_resource(getCa.Route(), &getCa);
    ws.register_resource(getCaCert.Route(), &getCaCert);
    ws.register_resource(issueCert.Route(),&issueCert);
    ws.start(true);

  } catch (std::exception &ex) {
    cout << ex.what() << endl;
  }
  return 0;
}
