package cz.cvut.fit.tjv.realestates.client.data;

import cz.cvut.fit.tjv.realestates.client.model.CustomerDTO;
import cz.cvut.fit.tjv.realestates.client.model.OwnerDTO;
import cz.cvut.fit.tjv.realestates.client.model.PropertyDTO;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.http.MediaType;
import org.springframework.stereotype.Component;
import org.springframework.web.reactive.function.client.WebClient;
import reactor.core.publisher.Flux;
import reactor.core.publisher.Mono;


@Component
public class PropertyClient {
    private static final String PROPERTY_URI = "/{id}";
    private final WebClient webClient;

    PropertyClient(@Value("${backend.url}") String backendUrl) {
        this.webClient = WebClient.create(backendUrl + "/rest/api/property");
    }

    public Mono<PropertyDTO> getPropertyById(Long id) {
        return webClient.get()
                .uri(PROPERTY_URI, id)
                .retrieve()
                .bodyToMono(PropertyDTO.class);
    }

    public Flux<PropertyDTO> getAllProperties() {
        return webClient.get()
                .retrieve()
                .bodyToFlux(PropertyDTO.class);
    }

    public Flux<PropertyDTO> getPropertiesOwnedByBothOwners(String emailFirstOwner, String emailSecondOwner) {
        return webClient.get()
                .uri(uriBuilder -> uriBuilder.path("/owners/intersection")
                        .queryParam("o1", emailFirstOwner)
                        .queryParam("o2", emailSecondOwner)
                        .build())
                .retrieve()
                .bodyToFlux(PropertyDTO.class);
    }

    public Mono<PropertyDTO> createProperty(PropertyDTO property) {
        return webClient.post()
                .contentType(MediaType.APPLICATION_JSON)
                .bodyValue(property)
                .retrieve()
                .bodyToMono(PropertyDTO.class);
    }

    public Mono<PropertyDTO> updatePropertyById(PropertyDTO property, Long id) {
        return webClient.put()
                .uri(PROPERTY_URI, id)
                .contentType(MediaType.APPLICATION_JSON)
                .bodyValue(property)
                .retrieve()
                .bodyToMono(PropertyDTO.class);
    }

    public Mono<Void> deleteProperty(Long id) {
        return webClient.delete()
                .uri(PROPERTY_URI, id)
                .retrieve()
                .bodyToMono(Void.class);
    }

    public Mono<CustomerDTO> addCustomerToProperty(Long id, CustomerDTO customer) {
        return webClient.put()
                .uri(PROPERTY_URI + "/customer", id)
                .contentType(MediaType.APPLICATION_JSON)
                .bodyValue(customer)
                .retrieve()
                .bodyToMono(CustomerDTO.class);
    }

    public Mono<Void> removeCustomerFromProperty(Long id) {
        return webClient.delete()
                .uri(PROPERTY_URI + "/customer", id)
                .retrieve()
                .bodyToMono(Void.class);

    }

    public Flux<OwnerDTO> addOwnerToProperty(Long id, OwnerDTO owner) {
        return webClient.put()
                .uri(PROPERTY_URI + "/owners", id)
                .contentType(MediaType.APPLICATION_JSON)
                .bodyValue(owner)
                .retrieve()
                .bodyToFlux(OwnerDTO.class);
    }

    public Mono<Void> removeOwnerFromProperty(Long id, String email) {
        return webClient.delete()
                .uri(PROPERTY_URI + "/owners/" + email, id)
                .retrieve()
                .bodyToMono(Void.class);
    }
}
